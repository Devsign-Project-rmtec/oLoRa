var byte = [0, 15, 9, 130, 17, 168, 62, 14, 63, 76, 10, 56, 13, 0, 255, 255];

var status = byte[0];
var latitude_sign = (byte[4]>>7) & 0x01;
var latitude_int = (byte[4] & 0x7F) << 1 | ((byte[3] >> 7) & 0x01);
var latitude_dec = (byte[3] & 0x7F) << 16 | byte[2] << 8 | byte[1];

var longitude_sign = (byte[8]>>7) & 0x01;
var longitude_int = (byte[8] & 0x7F) << 1 | ((byte[7] >> 7) & 0x01);
var longitude_dec = (byte[7] & 0x7F) << 16 | byte[6] << 8 | byte[5];

console.log(latitude_sign);
console.log(latitude_int);
console.log(latitude_dec);

console.log(longitude_sign);
console.log(longitude_int);
console.log(longitude_dec);