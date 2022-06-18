extern crate mustache;

use mustache::MapBuilder;
use std::env;
use std::error::Error;
use std::fs;
use std::io;
use std::process::Command;

fn pairs<I>(data: I) -> impl Iterator<Item = (I::Item, I::Item)>
where
    I: Iterator + Clone,
{
    let left = data.clone().step_by(2);
    let right = data.skip(1).step_by(2);
    left.zip(right)
}

fn main() -> Result<(), Box<dyn Error>> {
    let all_args: Vec<String> = env::args().skip(1).collect();
    let (source_path, kv_args) = all_args
        .split_first()
        .ok_or("mst: Requires at least 1 argument: path to template file")?;

    let template: mustache::Template = mustache::compile_path(&source_path)
        .unwrap_or_else(|err| panic!("mustache: Can't compile template: {:?}", err));

    let data: mustache::Data = {
        let mut data = MapBuilder::new();
        for (k, v) in pairs(kv_args.iter()) {
            data = data.insert_str(k, v);
        }

        data = data
            .insert_fn("env", move |env_name| {
                env::var(&env_name)
                    .unwrap_or_else(|err| panic!("env: Can't read ${:?}: {:?}", env_name, err))
            })
            .insert_fn("maybe_env", move |env_name| {
                env::var(env_name).unwrap_or("".to_string())
            });

        if cfg!(feature = "with_include") {
            data = data.insert_fn("include", move |filename| {
                fs::read_to_string(&filename)
                    .unwrap_or_else(|err| panic!("include: Can't read {:?}: {:?}", filename, err))
            });
        }

        if cfg!(feature = "with_cmd") {
            data = data.insert_fn("cmd", move |cmd| {
                let output = Command::new("sh")
                    .args(["-c", &cmd])
                    .output()
                    .unwrap_or_else(|err| panic!("cmd: Can't run {:?}: {:?}", cmd, err));
                if output.status.success() {
                    String::from_utf8(output.stdout).unwrap_or_else(|err| {
                        panic!("cmd: Can't decode stdout of {:?}: {:?}", cmd, err)
                    })
                } else {
                    let exitcode = output.status.code().map_or(
                        "process killed by some signal, no return code".to_string(),
                        |x| x.to_string(),
                    );
                    panic!("cmd: Execution error of {:?}: {:?}", cmd, exitcode)
                }
            });
        }
        data.build()
    };

    template
        .render_data(&mut io::stdout(), &data)
        .unwrap_or_else(|err| panic!("Can't render data: {:?}", err));
    Ok(())
}
