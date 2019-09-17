var bytes = [0, 178, 33, 130, 17, 195, 62, 14, 63, 100, 246, 183, 19, 0, 0, 0];

function dec2bin(dec){
    return (dec >>> 0).toString(2);
}

var status = bytes[0];
var latitude_sign = (bytes[4] >> 7) & 0x01;
var latitude_int = (bytes[4] & 0x7F) << 1 | ((bytes[3] >> 7) & 0x01);
var latitude_dec = (bytes[3] & 0x7F) << 16 | (bytes[2] & 0xFF) << 8 | bytes[1] & 0xFF;
var latitude_str = "";
if (latitude_sign == 0) latitude_str += '+';
else latitude_str += '-';
latitude_str += latitude_int.toString() + '.' + latitude_dec.toString();

var longitude_sign = (bytes[8] >> 7) & 0x01;
var longitude_int = (bytes[8] & 0x7F) << 1 | ((bytes[7] >> 7) & 0x01);
var longitude_dec = (bytes[7] & 0x7F) << 16 | bytes[6] << 8 | bytes[5];
var longitude_str = "";
if (longitude_sign == 0) longitude_str += '+';
else longitude_str += '-';
longitude_str += longitude_int.toString() + '.' + longitude_dec.toString();

var temperature = (bytes[10] << 8 | bytes[9]);
var humidity = (bytes[12] << 8 | bytes[11]) / 100;
if (temperature & 0x8000) {
    temperature = ~temperature + 1;
    temperature &= 0x7FFF;
    temperature *= -1;
}
temperature /= 100;

var battery = bytes[13];
var radiation = (bytes[15] << 8 | bytes[14]) / 100;


console.log(status.toString() + '/' + latitude_str + '/' + longitude_str + '/' + temperature.toString() + '/' + humidity.toString() + '/' + battery.toString() + '/' + radiation.toString());
