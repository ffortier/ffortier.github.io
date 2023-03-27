use std::{
    fmt::{Arguments, Display},
    str::FromStr,
};

use parser::Token;
pub use rendering::style::{Color, Format, Style};

pub mod error;
mod parser;
mod rendering;

pub struct Regex {
    tok: Token,
}

struct StyledOutput<'a, F>
where
    F: Fn(&Style, &Arguments<'_>) -> String + 'a,
{
    tok: &'a Token,
    style_func: F,
}

impl<'a, F> Display for StyledOutput<'a, F>
where
    F: Fn(&Style, &Arguments<'_>) -> String + 'a,
{
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        writeln!(
            f,
            "{}",
            (self.style_func)(
                &Style::default(),
                &format_args!("{}", rendering::render_token(&self.tok))
            )
        )
    }
}

impl Regex {
    pub fn with_style<'a, F>(&'a self, style_func: F) -> Box<dyn Display + 'a>
    where
        F: Fn(&Style, &Arguments<'_>) -> String + 'a,
    {
        Box::new(StyledOutput {
            tok: &self.tok,
            style_func,
        })
    }
}

impl Display for Regex {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        writeln!(f, "{}", rendering::render_token(&self.tok))
    }
}

impl FromStr for Regex {
    type Err = error::Error;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let tok = parser::parse_expr(s.chars())?;

        Ok(Self { tok })
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_phone_number_display() {
        let re: Regex = r#"^(\+\d{1,2}\s)?\(?\d{3}\)?[a-z\s.-]\d{3}[\s.-]\d{4}$"#
            .parse()
            .expect("parse");

        format!("{re}");
    }

    #[test]
    fn test_hello_display() {
        let re: Regex = r#"hello (?:\W+|[0-9])+"#.parse().expect("parse");

        format!("{re}");
    }
}
