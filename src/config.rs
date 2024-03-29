//! The configuration of the Helium compiler application.
//! 
//! # Config
//! [Config] represents the configuration of the Helium compiler application, like input and output file paths, optimization level, executable format and so on.
//! Config is generated by passing a ```Vec``` of ```String``` arguments to [Config::from_args()].
//!
//! # Example
//! ```
//! use std::env;
//! use helium::config::Config;
//!
//! let args: Vec<String> = env::args().collect();
//! let config = Config::from_args(&args);
//! ```

use std::error::Error;
use std::fmt;
use std::path::Path;
use std::str::FromStr;

use anyhow::{anyhow, Context, Result};

/// The configuration of the Helium compiler application, like input and output file paths, optimization level, executable format and so on.
///
/// # Examples
/// Generate a config from command-line arguments.
/// ```
/// use std::env;
/// use helium::config::Config;
///
/// let args: Vec<String> = env::args().collect();
/// let config = Config::from_args(&args);
/// ```
#[derive(Debug, PartialEq, Eq)]
pub struct Config<'a> {
    /// Input file path, default value is ```main.he```
    pub input_file_path: &'a Path,
    /// Output file path, default value is ```main.exe```
    pub output_file_path: &'a Path,
}

struct ConfigPrototype<'a> {
    input_file_path: Option<&'a Path>,
    output_file_path: Option<&'a Path>,
}

enum OptionSpecifier {
    None,
    InputFile,
    OutputFile,
}

#[derive(Debug, PartialEq, Eq)]
struct ParseOptionSpecifierError;

impl<'a> Config<'a> {
    /// Generates a config from provided arguments. Arguments should be of the form ```-option value```.
    /// Config options which weren't specified in the provided arguments are filled in with default values.
    ///
    /// # Examples
    /// ```
    /// use std::path::Path;
    ///
    /// use helium::vec_of_strings;
    /// use helium::config::Config;
    ///
    /// let args = vec_of_strings!["helium", "-i", "file.he", "-o", "file.exe"];
    /// let config = Config::from_args(&args).unwrap();
    ///
    /// assert_eq!(config, Config {
    ///     input_file_path: Path::new("file.he"),
    ///     output_file_path: Path::new("file.exe"),
    /// });
    /// ```
    /// # Errors
    /// Returns an error when the format of provided arguments is incorrect.
    /// # Panics
    /// Panics when ```args.len() < 1```.
    pub fn from_args(args: &'a [String]) -> Result<Self> {
        let mut config_prot = ConfigPrototype::empty();
        let mut option_specifier = OptionSpecifier::None;

        for arg in args.iter().skip(1) {
            let arg = arg.as_str();

            match option_specifier {
                OptionSpecifier::InputFile => {
                    if config_prot.input_file_path.is_some() {
                        return Err(anyhow!("input file path already specified"));
                    }

                    config_prot.input_file_path = Some(Path::new(arg));
                    option_specifier = OptionSpecifier::None;
                }
                OptionSpecifier::OutputFile => {
                    if config_prot.output_file_path.is_some() {
                        return Err(anyhow!("output file path already specified"));
                    }

                    config_prot.output_file_path = Some(Path::new(arg));
                    option_specifier = OptionSpecifier::None;
                }
                OptionSpecifier::None => {
                    option_specifier =
                        OptionSpecifier::from_str(arg).context(format!("unknown option: {arg}"))?;
                }
            }
        }

        let config = Config::from_prototype(config_prot);

        if let OptionSpecifier::None = option_specifier {
            Ok(config)
        } else {
            Err(anyhow!(
                "expected a value following option {}",
                args.last().unwrap()
            ))
        }
    }

    fn from_prototype(prototype: ConfigPrototype<'a>) -> Self {
        Self {
            input_file_path: prototype.input_file_path.unwrap_or(Path::new("main.he")),
            output_file_path: prototype.output_file_path.unwrap_or(Path::new("main.exe")),
        }
    }
}

impl ConfigPrototype<'_> {
    fn empty() -> Self {
        Self {
            input_file_path: None,
            output_file_path: None,
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
            "-i" | "--input-file" => Ok(OptionSpecifier::InputFile),
            "-o" | "--output-file" => Ok(OptionSpecifier::OutputFile),
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

        assert_eq!(
            config,
            Config {
                input_file_path: Path::new("main.he"),
                output_file_path: Path::new("main.exe"),
            }
        );

        let args = vec_of_strings![
            "helium",
            "--input-file",
            "app.he",
            "--output-file",
            "app.exe"
        ];
        let config = Config::from_args(&args).unwrap();

        assert_eq!(
            config,
            Config {
                input_file_path: Path::new("app.he"),
                output_file_path: Path::new("app.exe"),
            }
        );
    }

    #[test]
    fn sets_default_input_and_output_file() {
        let args = vec_of_strings!["helium"];
        let config = Config::from_args(&args).unwrap();

        assert_eq!(
            config,
            Config {
                input_file_path: Path::new("main.he"),
                output_file_path: Path::new("main.exe"),
            }
        );
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
