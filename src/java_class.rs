//
struct Class
{

}

impl Class
{

}

// 
pub struct ClassBuilder
{
    strings: Vec<String>
}

impl ClassBuilder
{
    pub fn new() -> ClassBuilder
    {
        return ClassBuilder{ 
            strings: Vec::new()
        };
    }

    fn build() -> Class
    {
        return Class{};
    }
}

//
#[cfg(test)]
mod tests
{
    #[test]
    fn test_add() {
        assert_eq!(3, 3);
    }
}