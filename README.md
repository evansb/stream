
#Stream.hpp

A simple C++ implementation of lazy list/generators/stream.

See example for usage.

##Example
As an example, here is how you describe
	
- A stream of integers.
- Starting from 0
- Stepping +1 each time
- Map to its squares.
- Filter those with 4 as last digit.


    	auto foo = Stream<int>::from(0)
                            .step([] (int x) { return x + 1; })
                            .map([] (int x) { return x * x; })
                            .filter([] (int x) { return x > 0
                                    && x % 10 == 4; });
    	auto foo20 = foo.take(20);
    	for (int i : foo20) { cout << i << endl; } // 4, 64, 144, etc.

## So basically
`.from` is the minimal definition of streams consisting of all same values.
You can then add `.map` and `.filter` as many as you want, but only the last `.step` will be considered.
## What you can do
Combine multiple maps and filters.

## What you can't do
Combine maps with filters, expecting those to work in as described in order. This library is able to compose all maps and filters sequentially, but not map-filter-map (You get the idea if you see the source).

## License
MIT