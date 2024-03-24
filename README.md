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

Prerequisite: gcc or clang supporting C++ 17, ninja, cmake, make and ssl. For example on Debian or Ubuntu system this should be enough:

```bash
sudo apt install build-essential g++ libssl-dev ninja-build cmake`
```

### Build and run it

```bash
make release
./build_release_cpu_native/bin/ecb_server --port 8080
```

It will fetch historical data from the ECB (default URLs are provided, call `ecb_server` without parameters to see all the options) and start listening on a given port.

### Options

There are two types of requests are accepted:

`/api/latest` - to get latest data
`/api/YYYY-MM-DD` - to get data from given day

The optional parameters:

- `base` - is the base currency (lower- or uppercase) - the values of other currencies are in relation to `base`. Default is _EUR_. Example: `usd`.
- `symbols` - coma-separated list of currencies to return. Example: `usd,nok,sek`. The result is simply filtered, so if the currency symbols are not existing then simply nothing will be returned.

### Getting data

On the other console (or via browser):

```bash
curl localhost:8080/api/latest
```

Result:

```json
{
 "base" : "EUR",
 "date" : "2024-03-22",
 "rates" :
 {
  "AUD" : 1.6600999999999999,
  "BGN" : 1.9558,
  "BRL" : 5.4099000000000004,
  "CAD" : 1.4688000000000001,
  "CHF" : 0.97289999999999999,
  "CNY" : 7.8244999999999996,
  "CZK" : 25.370999999999999,
  "DKK" : 7.4581,
  "GBP" : 0.85794999999999999,
  "HKD" : 8.4644999999999992,
  "HUF" : 396.30000000000001,
  "IDR" : 17111.810000000001,
  "ILS" : 3.9264999999999999,
  "INR" : 90.409000000000006,
  "ISK" : 148.90000000000001,
  "JPY" : 163.74000000000001,
  "KRW" : 1453.6199999999999,
  "MXN" : 18.1372,
  "MYR" : 5.1273999999999997,
  "NOK" : 11.617000000000001,
  "NZD" : 1.802,
  "PHP" : 60.927999999999997,
  "PLN" : 4.3170000000000002,
  "RON" : 4.9726999999999997,
  "SEK" : 11.4025,
  "SGD" : 1.4593,
  "THB" : 39.298000000000002,
  "TRY" : 34.620899999999999,
  "USD" : 1.0823,
  "ZAR" : 20.559699999999999
 },
 "success" : true
}
```

Or with parameters:

```bash
curl 'localhost:8080/api/latest?base=usd&symbols=eur,nok,dkk'
```

Result:

```json
{
 "base" : "USD",
 "date" : "2024-03-22",
 "rates" :
 {
  "DKK" : 6.8909729280236531,
  "EUR" : 0.92395823708768365,
  "NOK" : 10.733622840247621
 },
 "success" : true
}
```
