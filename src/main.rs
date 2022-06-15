extern crate mustache;

use mustache::MapBuilder;
use std::env;
use std::error::Error;
use std::fs;
use std::io;

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
        .ok_or("Requires at least 1 argument: path to template file")?;

    let template = mustache::compile_path(&source_path)
        .unwrap_or_else(|err| panic!("Can't compile template: {:?}", err));

    let data = {
        let mut data = MapBuilder::new();
        for (k, v) in pairs(kv_args.iter()) {
            data = data.insert_str(k, v);
        }

        data = data
            .insert_str("MST_SOURCE", source_path)
            .insert_fn("include", move |filename| {
                fs::read_to_string(&filename)
                    .unwrap_or_else(|err| panic!("Can't read file {:?}: {:?}", filename, err))
            });

        // TODO: cmd (flag)
        // TODO: maybe_env
        // TODO: env

        data.build()
    };

    template
        .render_data(&mut io::stdout(), &data)
        .unwrap_or_else(|err| panic!("Can't render data: {:?}", err));
    Ok(())
}
