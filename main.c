#include <stdio.h>
#include <stdlib.h>


// Constants
#define MOD_K 3
#define MOD_N 4
#define MOD_Q 97

// Prototypes
static void custom_polynomial_multiply(short (*result)[MOD_N], const short (*a)[MOD_K][MOD_N], const short (*b)[MOD_K], int i, int j, int s, int add_to_result);
static void custom_polynomial_multiply_single(short (*result), const short (*a)[MOD_N], const short (*b)[MOD_K], int i, int s, int add_to_result);
void generate_random(short (*v)[MOD_N], short len, int apply_mod);
void generate_toy_params(short (*A)[MOD_K][MOD_N], short (*t)[MOD_N], short (*s)[MOD_N]);
void encrypt(short (*A)[MOD_K][MOD_N], short (*t)[MOD_N], short (*u)[MOD_N], short (*v), short plain);
short decrypt(short (*s)[MOD_N], short (*u)[MOD_N], short (*v));
void print(short (*v)[MOD_N], char g);


int main() {
    short SS[MOD_K][MOD_N], AA[MOD_K][MOD_K][MOD_N], TT[MOD_K][MOD_N];
    generate_toy_params(AA, TT, SS);

    short U[MOD_K][MOD_N], V[MOD_N];
    short plain;
    printf("Enter a number less than 16: ");
    scanf("%d", &plain);

    encrypt(AA, TT, U, V, plain);
    printf("Encryption Result: \n");
    print(U, 'U');
    printf("\nV \n");
    for (int j = 0; j < MOD_N; j++) {
        printf("%d\t", V[j]);
    }
    printf("\n");

    short result = decrypt(SS, U, V);

    printf("\nDecryption Result: %d", result);
    printf("\n");

    return 0;
}


// Functions

// Function for polynomial multiplication in a naive way
static void custom_polynomial_multiply(short (*result)[MOD_N], const short (*a)[MOD_K][MOD_N], const short (*b)[MOD_K], int i, int j, int s, int add_to_result) {
    if (!add_to_result) {
        result[i][0] = (a[i][j][0] * b[s][0] - a[i][j][3] * b[s][1] - a[i][j][2] * b[s][2] - a[i][j][1] * b[s][3]) % MOD_Q;
        result[i][1] = (a[i][j][1] * b[s][0] + a[i][j][0] * b[s][1] - a[i][j][3] * b[s][2] - a[i][j][2] * b[s][3]) % MOD_Q;
        result[i][2] = (a[i][j][2] * b[s][0] + a[i][j][1] * b[s][1] + a[i][j][0] * b[s][2] - a[i][j][3] * b[s][3]) % MOD_Q;
        result[i][3] = (a[i][j][3] * b[s][0] + a[i][j][2] * b[s][1] + a[i][j][1] * b[s][2] + a[i][j][0] * b[s][3]) % MOD_Q;
    } else {
        result[i][0] = (result[i][0] + (a[i][j][0] * b[s][0] - a[i][j][3] * b[s][1] - a[i][j][2] * b[s][2] - a[i][j][1] * b[s][3])) % MOD_Q;
        result[i][1] = (result[i][1] + (a[i][j][1] * b[s][0] + a[i][j][0] * b[s][1] - a[i][j][3] * b[s][2] - a[i][j][2] * b[s][3])) % MOD_Q;
        result[i][2] = (result[i][2] + (a[i][j][2] * b[s][0] + a[i][j][1] * b[s][1] + a[i][j][0] * b[s][2] - a[i][j][3] * b[s][3])) % MOD_Q;
        result[i][3] = (result[i][3] + (a[i][j][3] * b[s][0] + a[i][j][2] * b[s][1] + a[i][j][1] * b[s][2] + a[i][j][0] * b[s][3])) % MOD_Q;
    }
}

// Function for single polynomial multiplication
static void custom_polynomial_multiply_single(short (*result), const short (*a)[MOD_N], const short (*b)[MOD_K], int i, int s, int add_to_result) {
    // Calculate product of coefficients
    if (!add_to_result) {
        result[0] = (a[i][0] * b[s][0] - a[i][3] * b[s][1] - a[i][2] * b[s][2] - a[i][1] * b[s][3]) % MOD_Q;
        result[1] = (a[i][1] * b[s][0] + a[i][0] * b[s][1] - a[i][3] * b[s][2] - a[i][2] * b[s][3]) % MOD_Q;
        result[2] = (a[i][2] * b[s][0] + a[i][1] * b[s][1] + a[i][0] * b[s][2] - a[i][3] * b[s][3]) % MOD_Q;
        result[3] = (a[i][3] * b[s][0] + a[i][2] * b[s][1] + a[i][1] * b[s][2] + a[i][0] * b[s][3]) % MOD_Q;
    } else {
        // Add product of coefficients to result
        result[0] = (result[0] + (a[i][0] * b[s][0] - a[i][3] * b[s][1] - a[i][2] * b[s][2] - a[i][1] * b[s][3])) % MOD_Q;
        result[1] = (result[1] + (a[i][1] * b[s][0] + a[i][0] * b[s][1] - a[i][3] * b[s][2] - a[i][2] * b[s][3])) % MOD_Q;
        result[2] = (result[2] + (a[i][2] * b[s][0] + a[i][1] * b[s][1] + a[i][0] * b[s][2] - a[i][3] * b[s][3])) % MOD_Q;
        result[3] = (result[3] + (a[i][3] * b[s][0] + a[i][2] * b[s][1] + a[i][1] * b[s][2] + a[i][0] * b[s][3])) % MOD_Q;
    }
}

// Generates a random matrix with 'len' rows

void generate_random(short (*v)[MOD_N], short len, int apply_mod) {
    for (int i = 0; i < len; i++)
        for (int j = 0; j < MOD_N; j++) {
            v[i][j] = rand() % 2 + 1;
            if (apply_mod) {
                v[i][j] = (((v[i][j] & 1) - (v[i][j] >> 1 & 1))) % MOD_Q;
            }
        }
}

// Function to generate toy parameters
void generate_toy_params(short (*A)[MOD_K][MOD_N], short (*t)[MOD_N], short (*s)[MOD_N]) {
    short e[MOD_K][MOD_N] = {0}, ss[MOD_K][MOD_N], AA[MOD_K][MOD_K][MOD_N], TT[MOD_K][MOD_N];

    generate_random(e, MOD_K, 1);
    //print(e, 'e');
    generate_random(s, MOD_K, 1);
    // Perform polynomial multiplication and addition for t
    for (short i = 0; i < MOD_K; i++)
        for (short j = 0; j < MOD_K; j++)
            for (short k = 0; k < MOD_N; k++) {
                A[i][j][k] = rand() % MOD_Q;
            }

    for (int i = 0; i < MOD_K; i++) {
        custom_polynomial_multiply(t, A, s, i, 0, 0, 0);
        for (int j = 1; j < MOD_K; j++) {
            custom_polynomial_multiply(t, A, s, i, j, j, 1);
        }
        for (int k = 0; k < MOD_N; k++) {
            t[i][k] = (t[i][k] + e[i][k]) % MOD_Q;
        }
    }
}

// Function to perform encryption
void encrypt(short (*A)[MOD_K][MOD_N], short (*t)[MOD_N], short (*u)[MOD_N], short (*v), short plain) {
    short r[MOD_K][MOD_N] = {0}, e1[MOD_K][MOD_N] = {0}, e2[MOD_N] = {0};

    generate_random(r, MOD_K, 1);
    generate_random(e1, MOD_K, 1);
    for (int j = 0; j < MOD_N; j++) {
        e2[j] = rand() % 3;
        e2[j] = (((e2[j] & 1) - (e2[j] >> 1 & 1))) % MOD_Q;
    }
    //print(e1, 'e');

    short AT[MOD_K][MOD_K][MOD_N];
    // Multiply custom polynomial with matrix A
    for (int i = 0; i < MOD_K; i++) {
        for (int j = 0; j < MOD_K; j++) {
            for (int k = 0; k < MOD_N; k++) {
                AT[j][i][k] = A[i][j][k];
            }
        }
    }
// Add error term and plaintext to matrix t
    for (int i = 0; i < MOD_K; i++) {
        custom_polynomial_multiply(u, AT, r, i, 0, 0, 0);
        for (int j = 1; j < MOD_K; j++) {
            custom_polynomial_multiply(u, AT, r, i, j, j, 1);
        }
        for (int k = 0; k < MOD_N; k++) {
            u[i][k] = (u[i][k] + e1[i][k]) % MOD_Q;
        }
    }

    short msg[MOD_N];
    for (int i = 0; i < MOD_N; i++) {
        msg[i] = plain >> i & 1;
        msg[i] = msg[i] * 49;
    }

    custom_polynomial_multiply_single(v, t, r, 0, 0, 0);
    for (int j = 1; j < MOD_K; j++) {
        custom_polynomial_multiply_single(v, t, r, j, j, 1);
    }

    for (int k = 0; k < MOD_N; k++) {
        v[k] = (v[k] + e2[k] + msg[k]) % MOD_Q;
    }
}

// Function to perform decryption
short decrypt(short (*s)[MOD_N], short (*u)[MOD_N], short (*v)) {
    short p[MOD_N], temp[MOD_N];
    // Multiply s and u with coefficients 0, 0, and 1
    custom_polynomial_multiply_single(temp, s, u, 0, 0, 0);
    for (int j = 1; j < MOD_K; j++) {
        custom_polynomial_multiply_single(temp, s, u, j, j, 1);
    }

    short plain = 0;
    for (int i = 0; i < MOD_N; i++) {
        p[i] = (v[i] - temp[i]) % MOD_Q;
        printf("%d\t", p[i]);
    }
    printf("\n");
    for (int i = 0; i < MOD_N; i++) {
        short val = p[i];
        if (val > 49)
            val -= MOD_Q;
        short bit = abs(val) > (MOD_Q / 4);
        //printf("%d", bit);
        plain |= bit << (i);
    }

    return plain;
}

// Function to print a 2D array
void print(short (*v)[MOD_N], char g) {
    printf("%c", g);
    for (int i = 0; i < MOD_K; i++) {
        printf("\n");
        for (int j = 0; j < MOD_N; j++) {
            printf("%d\t", v[i][j]);
        }
    }
    printf("\n");
}


