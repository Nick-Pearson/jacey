use std::fs;
use std::io::Cursor;
use byteorder::{BigEndian, ReadBytesExt, WriteBytesExt};
use std::convert::TryFrom;
use std::io::prelude::*;

//
pub struct Class
{

}

impl Class
{

}

// 
pub struct ClassBuilder
{
    pub string_data: Vec<String>
}

impl ClassBuilder
{
    pub fn new() -> ClassBuilder
    {
        return ClassBuilder{ 
            string_data: Vec::new()
        };
    }

    pub fn load_file(&mut self, filepath : String)
    {
        self.load_bytes(fs::read(filepath).expect("Failed to read class file"));
    }

    fn load_bytes(&mut self, bytes : Vec<u8>)
    {
        let mut reader = Cursor::new(bytes);
        self.check_preamble(&mut reader);

        let num_constants = reader.read_u16::<BigEndian>().unwrap();

        for _x in 0..num_constants
        {
            self.parse_constant(&mut reader);
        }
    }

    fn check_preamble(&mut self, reader : &mut Cursor<Vec<u8>>)
    {
        let magic_num = reader.read_u32::<BigEndian>().unwrap();

        if magic_num != 0xcafebabe
        {
            panic!(".class file is corrupt, 0x{:X} != 0xCAFEBABE", magic_num);
        }

        let minor_ver = reader.read_u16::<BigEndian>().unwrap();
        let major_ver = reader.read_u16::<BigEndian>().unwrap();

        if major_ver != 52
        {
            panic!("unsupported class file version {}.{}", major_ver, minor_ver)
        }
    }

    fn parse_constant(&mut self, reader: &mut Cursor<Vec<u8>>)
    {
        let tag = reader.read_u8().unwrap();

        if tag == 8
        {
            let size = reader.read_u16::<BigEndian>().unwrap();
            let mut buffer = vec![0; size as usize];
            reader.read_exact(buffer.as_mut_slice());

            self.string_data.push(String::from_utf8(buffer).expect("error parsing utf-8 string"));
        }
        else
        {
            panic!("unnsuported constant type {}", tag);
        }
    }

    pub fn build(&self) -> Class
    {
        return Class{};
    }
}

//
#[cfg(test)]
mod tests
{
    use super::*;

    fn create_class(major_ver : u16, minor_ver : u16) -> Vec<u8>
    {
        let mut bytes = Vec::<u8>::with_capacity(20);
        bytes.write_u32::<BigEndian>(0xcafebabe).unwrap();

        bytes.write_u16::<BigEndian>(minor_ver).unwrap();
        bytes.write_u16::<BigEndian>(major_ver).unwrap();

        return bytes;
    }

    #[test]
    #[should_panic]
    fn test_fail_lower_version()
    {
        let mut cb = ClassBuilder::new();
        cb.check_preamble(&mut Cursor::new(create_class(12, 42)));
    }

    #[test]
    #[should_panic]
    fn test_fail_higher_version()
    {
        let mut cb = ClassBuilder::new();
        cb.check_preamble(&mut Cursor::new(create_class(103, 1)));
    }

    #[test]
    fn test_pass_correct_version()
    {
        let mut cb = ClassBuilder::new();
        cb.check_preamble(&mut Cursor::new(create_class(52, 12)));
    }

    #[test]
    #[should_panic]
    fn test_fail_invalid_magic_number()
    {
        let mut cb = ClassBuilder::new();
        let mut bytes = Vec::<u8>::new();
        bytes.write_u32::<BigEndian>(0xcaeebabe).unwrap();
        cb.check_preamble(&mut Cursor::new(bytes));
    }

    #[test]
    #[should_panic]
    fn test_panic_on_missing_file()
    {
        let mut cb = ClassBuilder::new();
        cb.load_file(String::from("invalid/file/path.class"));
    }

    #[test]
    fn test_load_constant_ascii_as_utf8()
    {
        let mut cb = ClassBuilder::new();
        let mut bytes = Vec::<u8>::with_capacity(20);
        let value = String::from("I am an ascii string");

        bytes.push(8);
        bytes.write_u16::<BigEndian>(u16::try_from(value.len()).expect("The test string is too large")).unwrap();
        bytes.extend_from_slice(&value.as_bytes());

        cb.parse_constant(&mut Cursor::new(bytes));
        assert_eq!(1, cb.string_data.len());
        assert_eq!(value, cb.string_data[0]);
    }

    #[test]
    fn test_load_constant_unicode_as_utf8()
    {
        let mut cb = ClassBuilder::new();
        let mut bytes = Vec::<u8>::with_capacity(20);
        let value = String::from("私はユニコード文字列です");

        bytes.push(8);
        bytes.write_u16::<BigEndian>(u16::try_from(value.len()).expect("The test string is too large")).unwrap();
        bytes.extend_from_slice(&value.as_bytes());

        cb.parse_constant(&mut Cursor::new(bytes));
        assert_eq!(1, cb.string_data.len());
        assert_eq!(value, cb.string_data[0]);
    }

    #[test]
    fn test_load_full_file()
    {
        let mut cb = ClassBuilder::new();
        cb.load_file(String::from("resources/sample0.class"));

        assert_eq!(16, cb.string_data.len());
        assert_eq!(String::from("<init>"), cb.string_data[0]);
        assert_eq!(String::from("()V"), cb.string_data[1]);
        assert_eq!(String::from("Code"), cb.string_data[2]);
        assert_eq!(String::from("LineNumberTable"), cb.string_data[3]);
        assert_eq!(String::from("main"), cb.string_data[4]);
        assert_eq!(String::from("([Ljava/lang/String;)V"), cb.string_data[5]);
        assert_eq!(String::from("SourceFile"), cb.string_data[6]);
        assert_eq!(String::from("sample0.java"), cb.string_data[7]);
        assert_eq!(String::from("Hello World"), cb.string_data[8]);
        assert_eq!(String::from("com/jacey/sample/sample0"), cb.string_data[9]);
        assert_eq!(String::from("java/lang/Object"), cb.string_data[10]);
        assert_eq!(String::from("java/lang/System"), cb.string_data[11]);
        assert_eq!(String::from("out"), cb.string_data[12]);
        assert_eq!(String::from("Ljava/io/PrintStream;"), cb.string_data[13]);
        assert_eq!(String::from("java/io/PrintStream"), cb.string_data[14]);
        assert_eq!(String::from("println"), cb.string_data[15]);
        assert_eq!(String::from("(Ljava/lang/String;)V"), cb.string_data[16]);
    }
}