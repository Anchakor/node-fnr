const addon = require('../build/Release/fnr-native');

interface IFnrNative
{
    encrypt(data: Buffer, passwd: Buffer, tweak_str: Buffer, data_count: number): void;
    decrypt(data: Buffer, passwd: Buffer, tweak_str: Buffer, data_count: number): void;
};

class Fnr {
    /**
     * Inicialize FCR cipher.
     * @param data_bitsize Size of data pieces to en/decrypt in bits. Must be between 1 and 128 (inclusive).
     * @param passwd_bitsize Size of the secret password in bits. 128, 192 or 256.
     * @param tweak_bytesize Size of the possibly public tweak in bytes.
     */
    constructor(data_bitsize: number, passwd_bitsize: 128|192|256, tweak_bytesize: number) {
        this._addonInstance = new addon.Fnr(data_bitsize, passwd_bitsize, tweak_bytesize);
    }

    /**
     * Encrypt data by the FCR cipher.
     * @param data Buffer of the data to encrypt (in place).
     * @param passwd Buffer of the secret password.
     * @param tweak_str Buffer of the possibly public tweak.
     * @param data_count Number of the data pieces to encrypt in the `data` buffer.
     */
    encrypt(data: Buffer, passwd: Buffer, tweak_str: Buffer, data_count: number) {
        this._addonInstance.encrypt(data, passwd, tweak_str, data_count);
    }

    /**
     * Decrypt data by the FCR cipher.
     * @param data Buffer of the data to decrypt (in place).
     * @param passwd Buffer of the secret password.
     * @param tweak_str Buffer of the possibly public tweak.
     * @param data_count Number of the data pieces to decrypt in the `data` buffer.
     */
    decrypt(data: Buffer, passwd: Buffer, tweak_str: Buffer, data_count: number) {
        this._addonInstance.decrypt(data, passwd, tweak_str, data_count);
    }

    // private members
    private _addonInstance: IFnrNative;
}

export = Fnr;
