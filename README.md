# ECB rates

This is a http server which serves current and historical currenty rates as JSON. Data is taken from [European Central Bank](https://www.ecb.europa.eu/stats/eurofxref/) and every 1h it is updated.

It was written just for fun to test some C++ 17 features and libraries.

## Quick start

Prerequisite: gcc or clang supporting C++ 17.

Build it:

```
git submodule update --init --recursive # to get all submodules
make release
```

Run it from build\_release... directory:
```
cd build_release.....
./bin/ecbrates -p 8080
```

It will fetch historical data from the ECB and start listening on given ports. Only 2 types of requests are accepted:
`/api/latest` - to get latest data
`/api/YYYY-MM-DD` - to get data from given day

The optional parameters, returned data and errors should be compatible with those of [fixer.io](https://fixer.io/documentation).

## Examples

Start the server:
```
./build_release/bin/ecbrates -p 8080
```

And on the other console (or via browser):
```
curl localhost:8080/api/latest

{
	"base" : "EUR",
	"date" : "2018-12-06",
	"rates" : 
	{
		"AUD" : 1.5745,
		"BGN" : 1.9558,
		"BRL" : 4.437,
		"CAD" : 1.5229,
		"CHF" : 1.1304,
        ...
		"USD" : 1.1351,
		"ZAR" : 15.9797
	},
	"success" : true
}
```

## Crosscompiling for arm

It is possible to cross compile it for some powerful enough arm devices like RaspberryPi. Assuming you have Debian OS
just install `g++-arm-linux-gnueabihf`, `qemu-user` and `libssl-dev:armhf` (notice it is taken from armhf architecture
which you must enable first using `dpkg --add-architecture`). A few words of explanation: we install
`g++-arm-linux-gnueabihf` for obvious reason. The `qemu-user` is needed for compiling libcurl or precisely for running
its tests/checks before compilation (see Makefile for options passed to cmake). And finally `libssl-dev:armhf` is needed
to link statically with.

When crosscompiling is used (i.e. you specify some ARMCPU option - see Makefile for details) then you will a statically
compiled binary with no tests.



