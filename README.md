# node-fnr

Node-js bindings for [libFNR](http://cisco.github.io/libfnr/) - A reference implementation for FNR encryption scheme.

FNR is a block cipher to encrypt small domain
objects (< 128 bits) like IPv4 addresses, MAC addreses,
Serial numbers, Account ids, Credit Card numbers etc.
while preserving their lengths (and formats if needed). 
Useful for data anonymization. 

See [Format-preserving encryption](https://en.wikipedia.org/wiki/Format-preserving_encryption).

## Installation

Requires C library `libfrn`, which in turn requires `openssl` > 1.0.1e (installed in the system as dynamic shared libraries).

```sh
npm install node-fnr
```

Tested on Linux. Shouldn't be hard to make work on other platforms (PRs welcome).

## Usage

```ts
const data_bitsize = 16; // ... other variables
const fnr = new Fnr(data_bitsize, passwd_bitsize, tweak_bytesize);

const input = [0,1,65535];

const data = Buffer.alloc(data_bytesize * input.length);
for (let index = 0; index < input.length; index++) {
	data.writeUInt16BE(input[index], index * data_bytesize);
}

fnr.encrypt(data, passwd, tweak_str, input.length);
// now `data` is encrypted, read it with data.readUInt16BE()
// decrypt method has the same signature
```

## Development

```sh
npm install # building
npm link typescript # linking global typescript
npm run test
```

Updating dependencies. Use:

```sh
npm outdated
```

Possibly update package.json for major upgrades, then run:

```sh
rm node_modules/typescript; npm update; npm install; npm link typescript
npm run test
```
