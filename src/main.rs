use std::env;

use anyhow::Result;

use helium::config::Config;

fn main() -> Result<()> {
    let args: Vec<String> = env::args().collect();
    let _config = Config::from_args(&args);

    Ok(())
}
