int encode(const char* inp, int inbytes, char* outp, int outbytes)
{
    if (inbytes == 0) {
        return 0;
    }
    int st[16] = { 0 };
    int st2[16] = { 0 };
    int j = 0;
    for (j = 0; j < inbytes; j++) {
        unsigned char x = (unsigned char)inp[j];
        unsigned char y = (0xf0 & x) >> 4;
        unsigned char z = (0x0f & x);
        st[y]++; st2[y]++;
        st[z]++; st2[z]++;
    }
    int rank[16] = { 0 };
    for (int i = 0; i < 15; i++) {
        for (int k = i + 1; k < 16; k++) {
            if (st2[k] > st2[i]) {
                int tmp = st2[k];
                st2[k] = st2[i];
                st2[i] = tmp;
            }
        }
    }
    int totalbit = 0;
    for (int i = 0; i <= 3; i++) {
        totalbit += 3 * st2[i];
    }
    for (int i = 4; i <= 7; i++) {
        totalbit += 4 * st2[i];
    }
    for (int i = 8; i <= 15; i++) {
        totalbit += 5 * st2[i];
    }
    totalbit += 36;
    int totalbyte;
    int padded = 0;

    if (totalbit % 8 == 0) {
        totalbyte = totalbit / 8;
        padded = 0;
    }
    else {
        totalbyte = totalbit / 8 + 1;
        if (totalbit % 8 == 1) padded = 7;
        else if (totalbit % 8 == 2) padded = 6;
        else if (totalbit % 8 == 3) padded = 5;
        else if (totalbit % 8 == 4) padded = 4;
        else if (totalbit % 8 == 5) padded = 3;
        else if (totalbit % 8 == 6) padded = 2;
        else if (totalbit % 8 == 7) padded = 1;
    }
    if (totalbyte > outbytes) {
        return -1;
    }
    for (int i = 0; i < 16; i++) {
        for (int k = 0; k < 16; k++) {
            if (st2[i] == st[k]) {
                rank[i] = k;
                st[k] = -1;
                break;
            }
        }
    }
    for (int i = 8; i < 15; i++) {
        for (int k = i + 1; k < 16; k++) {
            if (rank[k] < rank[i]) {
                int tmp = rank[k];
                rank[k] = rank[i];
                rank[i] = tmp;
            }
        }
    }
    unsigned char value[16][6] = { "000", "001", "010", "011", "1000", "1001", "1010","1011","11000","11001","11010","11011",
            "11100","11101","11110","11111" };

    for (int i = 0; i <= 3; i++) {
        outp[i] = (char)(((rank[2 * i]) << 4) | rank[2 * i + 1]);
    }

    unsigned char match[16][6] = { 0 };
    for (int i = 0; i < 16; i++) {
        for (int r = 0; r < 16; r++) {
            if (i == rank[r]) {
                int k;
                for (k = 0; value[r][k] != '\0'; k++) {
                    match[i][k] = value[r][k];
                }
                match[i][k] = '\0';
            }
        }
    }
    int buffer = 4;
    int inside_buf = 0;
    int now = (padded << 4);
    int outp_state = 4;
    int inp_state = 0;
    int final = (outbytes > totalbyte) ? totalbyte : outbytes;
    unsigned char bit_8[8] = { 0 };
    unsigned char arr[6] = { 0 };
    for (outp_state = 4; outp_state < final;) {

        unsigned char x = (unsigned char)inp[inp_state / 2];
        if (inp_state % 2 == 0) {
            int k;
            unsigned char r = ((0xf0 & x) >> 4);
            for (k = 0; match[r][k] != '\0'; k++) {
                arr[k] = match[r][k];
            }
            arr[k] = '\0';

        }
        else if (inp_state % 2 == 1) {
            int k;
            unsigned char r = (0x0f & x);
            for (k = 0; match[r][k] != '\0'; k++) {
                arr[k] = match[r][k];
            }
            arr[k] = '\0';
        }
        inside_buf = 0;
        for (inside_buf = 0; ; inside_buf++) {
            if (outp_state == final - 1) {
                for (int i = 7; i > 7 - padded; i--) {
                    bit_8[i] = '0';
                }
            }
            if ((outp_state == final - 1) && (buffer == 8 - padded)) {
                outp[outp_state] = (char)(((bit_8[7] - '0') | ((bit_8[6] - '0') << 1) | ((bit_8[5] - '0') << 2) | ((bit_8[4] - '0') << 3) | ((bit_8[3] - '0') << 4) |
                    ((bit_8[2] - '0') << 5) | ((bit_8[1] - '0') << 6) | ((bit_8[0] - '0') << 7)));
                outp_state++;
                break;
            }
            if (arr[inside_buf] == '\0') {
                inp_state++;

                break;
            }
            bit_8[buffer] = arr[inside_buf];
            if (buffer == 7) {
                buffer = 0;
                outp[outp_state] = (char)((bit_8[7] - '0') | ((bit_8[6] - '0') << 1) | ((bit_8[5] - '0') << 2) | ((bit_8[4] - '0') << 3) | ((bit_8[3] - '0') << 4) | ((bit_8[2] - '0') << 5) | ((bit_8[1] - '0') << 6) | ((bit_8[0] - '0') << 7) | now);
                for (int i = 0; i <= 7; i++) {
                    bit_8[i] = '0';
                }
                now = 0;
                outp_state++;
                if (outp_state == final) {

                    break;
                }
            }
            else {
                buffer++;
            }
        }

    }

    return totalbyte;
}
