use std::env;

use helium::config::Config;

fn main() {
    let args: Vec<String> = env::args().collect();
    let _config = Config::from_args(&args);
}
