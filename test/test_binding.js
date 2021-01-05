const Fnr = require("../dist/binding.js");
const assert = require("assert");

assert(Fnr, "The expected module is undefined");

function testBasic()
{
    const passwd = Buffer.from('GATTACASGATTACAS','ascii');
    const passwd_bitsize = 8*16;
    const tweak_str = Buffer.from('CADDILACCADDILAC','ascii');
    const tweak_bytesize = 16;
    
    const data_bitsize = 16;
    const data_bytesize = 2;

    const fnr = new Fnr(data_bitsize, passwd_bitsize, tweak_bytesize);
    assert(fnr.encrypt, "The expected encrypt method is not defined");
    assert(fnr.decrypt, "The expected decrypt method is not defined");

    const input = [0,1,5,6,65530,65535];
    const expected = [54688,24861,51179,22506,27990,25276];

    const data = Buffer.alloc(data_bytesize * input.length);
    for (let index = 0; index < input.length; index++) {
        data.writeUInt16BE(input[index], index * data_bytesize);
    }
    
    fnr.encrypt(data, passwd, tweak_str, input.length);
    for (let index = 0; index < input.length; index++) {
        const d = data.readUInt16BE(index * data_bytesize);
        assert(d == expected[index], `Encryped data doesn't match for record index ${index}`
            +` (expected ${expected[index]}, actual ${d})`);
    }
    
    fnr.decrypt(data, passwd, tweak_str, input.length);
    for (let index = 0; index < input.length; index++) {
        const d = data.readUInt16BE(index * data_bytesize);
        assert(d == input[index], `Decryped data doesn't match for record index ${index}`
        +` (expected ${input[index]}, actual ${d})`);
    }
}

function testInvalidParams()
{
    const instance = new Fnr();
}

assert.doesNotThrow(testBasic, undefined, "testBasic threw an expection");
assert.throws(testInvalidParams, undefined, "testInvalidParams didn't throw");

console.log("Tests passed - everything looks OK!");
