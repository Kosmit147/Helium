use std::path::Path;
use std::str::FromStr;
use std::fmt;
use std::error::Error;

use anyhow::{anyhow, Context, Result};

#[derive(Debug, PartialEq, Eq)]
pub struct Config<'a> {
    pub input_file: &'a Path,
    pub output_file: &'a Path,
}

struct ConfigPrototype<'a> {
    input_file: Option<&'a Path>,
    output_file: Option<&'a Path>,
}

enum OptionSpecifier {
    None,
    InputFile,
    OutputFile,
}

#[derive(Debug, PartialEq, Eq)]
pub struct ParseOptionSpecifierError;

impl<'a> Config<'a> {
    pub fn from_args(args: &'a [String]) -> Result<Self> {
        let mut config_prot = ConfigPrototype::empty();
        let mut option_specifier = OptionSpecifier::None;
        
        for arg in args.iter().skip(1) {
            let arg = arg.as_str();

            match option_specifier {
                OptionSpecifier::InputFile => {
                    if config_prot.input_file.is_some() {
                        return Err(anyhow!("input file already specified"));
                    }
                    
                    config_prot.input_file = Some(Path::new(arg));
                    option_specifier = OptionSpecifier::None;
                },
                OptionSpecifier::OutputFile => {
                    if config_prot.output_file.is_some() {
                        return Err(anyhow!("output file already specified"));
                    }

                    config_prot.output_file = Some(Path::new(arg));
                    option_specifier = OptionSpecifier::None;
                }
                OptionSpecifier::None => {
                    option_specifier = OptionSpecifier::from_str(arg).context(format!("unknown option specifier: {arg}"))?;
                },
            }
        }

        let config = Config::from_prototype(config_prot);

        if let OptionSpecifier::None = option_specifier {
            Ok(config)
        } else {
            Err(anyhow!("expected a value following option {}", args.last().unwrap()))
        }
    }

    fn from_prototype(prototype: ConfigPrototype<'a>) -> Self {
        Self {
            input_file: prototype.input_file.unwrap_or(Path::new("main.he")),
            output_file: prototype.output_file.unwrap_or(Path::new("main.exe")),
        }
    }
}

impl ConfigPrototype<'_> {
    fn empty() -> Self {
        Self {
            input_file: None,
            output_file: None,
        }
    }
}

impl Error for ParseOptionSpecifierError {}

impl fmt::Display for ParseOptionSpecifierError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "failed to parse option specifier")
    }
}

impl FromStr for OptionSpecifier {
    type Err = ParseOptionSpecifierError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "-i" | "--input-file" => {
                Ok(OptionSpecifier::InputFile)
            },
            "-o" | "--output-file" => {
                Ok(OptionSpecifier::OutputFile)
            },
            _ => Err(ParseOptionSpecifierError),
        }
    }
}

#[cfg(test)]
mod tests {    
    use super::*;
    use crate::vec_of_strings;

    #[test]
    fn parses_input_and_output_file() {
        let args = vec_of_strings!["helium", "-o", "main.exe", "-i", "main.he"];
        let config = Config::from_args(&args).unwrap();

        assert_eq!(config, Config {
            input_file: Path::new("main.he"),
            output_file: Path::new("main.exe"),
        });

        let args = vec_of_strings!["helium", "--input-file", "app.he", "--output-file", "app.exe"];
        let config = Config::from_args(&args).unwrap();

        assert_eq!(config, Config {
            input_file: Path::new("app.he"),
            output_file: Path::new("app.exe"),
        });
    }

    #[test]
    fn sets_default_input_and_output_file() {
        let args = vec_of_strings!["helium"];
        let config = Config::from_args(&args).unwrap();

        assert_eq!(config, Config {
            input_file: Path::new("main.he"),
            output_file: Path::new("main.exe"),
        });
    }

    #[test]
    fn returns_error_with_incorrect_args() {
        let args = vec_of_strings!["helium", "arg1", "arg2", "arg3"];
        let config = Config::from_args(&args);

        assert!(config.is_err());

        let args = vec_of_strings!["helium", "-o", "-i", "arg1", "arg2", "arg3"];
        let config = Config::from_args(&args);

        assert!(config.is_err());

        let args = vec_of_strings!["helium", "-o", "main.exe", "-o", "sth_else.exe"];
        let config = Config::from_args(&args);

        assert!(config.is_err());

        let args = vec_of_strings!["helium", "-o", "main.exe", "-o"];
        let config = Config::from_args(&args);

        assert!(config.is_err());
    }
}