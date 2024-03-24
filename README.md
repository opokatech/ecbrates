# ECB rates

This repository hosts a few utility programs for handling historical currencies rates obtained from
the [European Central Bank](https://www.ecb.europa.eu/stats/eurofxref/).

One (`ecb_fetcher`) can fetch either historical or daily data, show it on the screen and optionally save as XML to a file.

Another one is a simple http server which serves data as JSON.
It can load initial data from a given file or from give URL (there are reasonable defaults).
On receiving `SIGUSR1` it can fetch more data from given URL (again, there is a default address for it). This can be used
in a cron script to do it periodically.

It was written just for fun back in 2018 to test some C++ 17 features and libraries. And refactored in 2024.

## Quick start

Prerequisite: gcc or clang supporting C++ 17.

Build it:

```bash
git submodule update --init --recursive
make release
```

Run server:

```bash
make run
```

It will fetch historical data from the ECB and start listening on a given port.
Only 2 types of requests are accepted:

`/api/latest` - to get latest data
`/api/YYYY-MM-DD` - to get data from given day

The optional parameters:

- `base` - is the base currency - the values of other currencies are in relation to `base`,
- `symbols` - coma separated list of currencies to return.

## Examples

Start the server:

```bash
make run_server
```

And on the other console (or via browser):

```bash
curl localhost:8080/api/latest
```

```json
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

## Cross compiling for arm

It is possible to cross compile it for some powerful enough arm devices like RaspberryPi. Assuming you have Debian OS
just install `g++-arm-linux-gnueabihf`, `qemu-user` and `libssl-dev:armhf` (notice it is taken from armhf architecture
which you must enable first using `dpkg --add-architecture`). A few words of explanation: we install
`g++-arm-linux-gnueabihf` for obvious reason. The `qemu-user` is needed for compiling libcurl or precisely for running
its tests/checks before compilation (see Makefile for options passed to cmake). And finally `libssl-dev:armhf` is needed
to link statically with.

When cross compiling is used (i.e. you specify some ARM CPU option - see Makefile for details) then you will a statically
compiled binary with no tests.
