
/**
 * Util 클래스
 */
export class Util {
    constructor() { }

    /** c++의 utc time_t 값을 입력값으로 사용 */
    static toLocalDate(utcSecond: number): Date {
        // time_t는 sec 까지만 표현되므로 msec 을 위해 * 1000
        let utc = new Date(utcSecond * 1000);

        let local = new Date(utc.getTime() + utc.getTimezoneOffset() * 60 * 1000);
        local.setHours(utc.getHours() - (utc.getTimezoneOffset() / 60));
        return local;
    }

    /** bytes 를 human readable 형식으로 변환 */
    static readableBytes(bytes: number, precision = 1): string {
        var units = ['B', 'KB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB'];

        var unit = 0;
        while (bytes >= 1024) {
            bytes /= 1024;
            unit++;
        }

        return bytes.toFixed(+ precision) + ' ' + units[unit];
    };

    /** 백분율 구하기 */
    static percentage(numerator: number, denominator: number): string {
        return ((numerator * 100) / denominator).toFixed(0).toString();
    }

    /** convert Date to 'YYYY-MM-DD hh:mm:ss */
    static toDateString(date: Date): string {
        return [
            this.leadingZero(date.getFullYear(), 4), '-',
            this.leadingZero(date.getMonth() + 1, 2), '-',
            this.leadingZero(date.getDate(), 2), ' ',
            this.leadingZero(date.getHours(), 2), ':',
            this.leadingZero(date.getMinutes(), 2), ':',
            this.leadingZero(date.getSeconds(), 2)
        ].join('');
    }

    /** 자릿수에 맞게 앞자리를 0으로 채움 */
    static leadingZero(n: number, digits: number): string {
        let zero = '';
        let nstr = n.toString();

        if (nstr.length < digits) {
            for (let i = 0; i < digits - nstr.length; i++) {
                zero += '0';
            }
        }
        return zero + n;
    }


}

