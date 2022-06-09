extern crate mustache;

use mustache::MapBuilder;
use std::env;
use std::error::Error;
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
    let args: Vec<_> = env::args().skip(1).collect(); // args without binary name
    let source_path = args
        .first()
        .ok_or("first argument needs to be a file path")?;
    let template = mustache::compile_path(source_path)?;

    let data = {
        let mut data = MapBuilder::new();
        for (k, v) in pairs(args.iter().skip(1)) {
            data = data.insert_str(k, v);
        }

        data = data.insert_str("MST_SOURCE", source_path);

        // TODO: include (flag)
        // TODO: cmd (flag)
        // TODO: maybe_env
        // TODO: env

        data.build()
    };

    template.render_data(&mut io::stdout(), &data)?;
    Ok(())
}
