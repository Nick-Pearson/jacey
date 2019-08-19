extern crate jacey;

use jacey::java_class::*;

#[test]
fn load_class_sample_0() 
{
    let mut cb = ClassBuilder::new();

    cb.load_file(String::from("resources/sample0.class"));

    let class = cb.build();
}

