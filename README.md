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
 "date" : "2024-03-27",
 "rates" :
 {
    "AUD" : 1.66,
    "BGN" : 1.9558,
    "BRL" : 5.3969,
    "CAD" : 1.4712,
    "CHF" : 0.9811,
    "CNY" : 7.8182,
    "CZK" : 25.319,
    "DKK" : 7.4582,
    "GBP" : 0.85768,
    "HKD" : 8.4615,
    "HUF" : 395.4,
    "IDR" : 17194.2,
    "ILS" : 3.9582,
    "INR" : 90.134,
    "ISK" : 149.9,
    "JPY" : 163.52,
    "KRW" : 1461.31,
    "MXN" : 17.9367,
    "MYR" : 5.1192,
    "NOK" : 11.6825,
    "NZD" : 1.8043,
    "PHP" : 60.849,
    "PLN" : 4.3148,
    "RON" : 4.9721,
    "SEK" : 11.506,
    "SGD" : 1.4586,
    "THB" : 39.381,
    "TRY" : 34.9005,
    "USD" : 1.0816,
    "ZAR" : 20.4716
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
  "date" : "2024-03-27",
  "rates" :
  {
    "DKK" : 6.89553,
    "EUR" : 0.924556,
    "NOK" : 10.8011
  },
  "success" : true
}
```
