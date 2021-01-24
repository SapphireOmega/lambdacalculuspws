use std::fmt::Display;

fn lazy_add<A, B>(a: A, b: B) -> Box<dyn Fn() -> i32>
where
    A: Fn() -> i32 + 'static,
    B: Fn() -> i32 + 'static,
{
    Box::new(move || a() + b())
}

fn hang() -> i32 {
    hang()
}

fn first<A, B>(a: A, b: B) -> Box<dyn Fn() -> i32>
where
    A: Fn() -> i32 + 'static,
    B: Fn() -> i32 + 'static,
{
    Box::new(a)
}

fn and<A, B>(a: A, b: B) -> Box<dyn Fn() -> bool>
where
    A: Fn() -> bool + 'static,
    B: Fn() -> bool + 'static,
{
    if a() { Box::new(b) } else { Box::new(a) }
}

struct List<T> {
    head: Option<Box<dyn Fn() -> T>>,
    tail: Option<Box<dyn Fn() -> List<T>>>,
}

impl<T: Copy + Display> List<T> {
    fn new() -> Box<dyn Fn() -> Self> {
        Box::new(move || List { head: None, tail: None })
    }

    fn from_slice(slice: &'static[T]) -> Box<dyn Fn() -> Self> {
        if slice.len() == 0 {
            List::new()
        } else {
            let index: T = slice[0];
            Box::new(
                move || List {
                    head: Some(Box::new(move || index)),
                    tail: Some(List::from_slice(&slice[1..])),
                }
            )
        }
    }

    fn print(&self) {
        if let Some(head) = &self.head {
            print!("{} ", head());
        } else {
            println!();
        }

        if let Some(tail) = &self.tail {
            tail().print();
        }
    }

    fn print_n(&self, n: usize) {
        if n == 0 {
            println!();
            return;
        }

        if let Some(head) = &self.head {
            print!("{} ", head());
        }

        if let Some(tail) = &self.tail {
            tail().print_n(n - 1);
        }
    }
}

fn range_from(begin: i32) -> Box<dyn Fn() -> List<i32>> {
    Box::new(
        move || List {
            head: Some(Box::new(move || begin)),
            tail: Some(range_from(begin + 1)),
        }
    )
}

fn main() {
    List::from_slice(&[3, 5, 4, 6, 9, 2])().print();
    range_from(0)().print_n(10);
}
