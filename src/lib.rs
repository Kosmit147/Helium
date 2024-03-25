//! # Helium
//! A simple compiler written in Rust.

pub mod config;

/// Creates a ```Vec``` of ```String``` elements from string slices.
///
/// # Examples
/// ```
/// use helium::vec_of_strings;
///
/// let names = vec_of_strings!["Mark", "Alice"];
///
/// assert_eq!(names[0], String::from("Mark"));
/// assert_eq!(names[1], String::from("Alice"));
///
/// use std::any::Any;
/// assert!(names.type_id() == Vec::<String>::new().type_id());
/// ```
#[macro_export]
macro_rules! vec_of_strings {
    ($($x:expr),*) => (vec![$($x.to_string()),*]);
}
