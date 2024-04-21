#include "aes.h"

void KeyExpansion(const uint8_t *Key, uint8_t *RoundKey) {
  unsigned i, k;
  uint8_t tempa[4]; // Used for the column/row operations

  // The first round key is the key itself.
  for (i = 0; i < Nk; ++i) {
    RoundKey[(i * 4) + 0] = Key[(i * 4) + 0];
    RoundKey[(i * 4) + 1] = Key[(i * 4) + 1];
    RoundKey[(i * 4) + 2] = Key[(i * 4) + 2];
    RoundKey[(i * 4) + 3] = Key[(i * 4) + 3];
  }

  // All other round keys are found from the previous round keys.
  for (i = Nk; i < Nb * (Nr + 1); ++i) {
    {
      tempa[0] = RoundKey[(i - 1) * 4 + 0];
      tempa[1] = RoundKey[(i - 1) * 4 + 1];
      tempa[2] = RoundKey[(i - 1) * 4 + 2];
      tempa[3] = RoundKey[(i - 1) * 4 + 3];

      if (i % Nk == 0) {
        {
          k = tempa[0];
          tempa[0] = tempa[1];
          tempa[1] = tempa[2];
          tempa[2] = tempa[3];
          tempa[3] = k;
        }

        {
          tempa[0] = sbox[tempa[0]];
          tempa[1] = sbox[tempa[1]];
          tempa[2] = sbox[tempa[2]];
          tempa[3] = sbox[tempa[3]];
        }

        tempa[0] = tempa[0] ^ Rcon[i / Nk];
      }
    }
    RoundKey[i * 4 + 0] = RoundKey[(i - Nk) * 4 + 0] ^ tempa[0];
    RoundKey[i * 4 + 1] = RoundKey[(i - Nk) * 4 + 1] ^ tempa[1];
    RoundKey[i * 4 + 2] = RoundKey[(i - Nk) * 4 + 2] ^ tempa[2];
    RoundKey[i * 4 + 3] = RoundKey[(i - Nk) * 4 + 3] ^ tempa[3];
  }
}

void SubBytes(state_t *state) {
  uint8_t i, j;
  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      (*state)[j][i] = sbox[(*state)[j][i]];
}

void InvSubBytes(state_t *state) {
  uint8_t i, j;
  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      (*state)[j][i] = rsbox[(*state)[j][i]];
}

void ShiftRows(state_t *state) {
  uint8_t temp;

  // Rotate first row 1 columns to left
  temp = (*state)[0][1];
  (*state)[0][1] = (*state)[1][1];
  (*state)[1][1] = (*state)[2][1];
  (*state)[2][1] = (*state)[3][1];
  (*state)[3][1] = temp;

  // Rotate second row 2 columns to left
  temp = (*state)[0][2];
  (*state)[0][2] = (*state)[2][2];
  (*state)[2][2] = temp;

  temp = (*state)[1][2];
  (*state)[1][2] = (*state)[3][2];
  (*state)[3][2] = temp;

  // Rotate third row 3 columns to left
  temp = (*state)[0][3];
  (*state)[0][3] = (*state)[3][3];
  (*state)[3][3] = (*state)[2][3];
  (*state)[2][3] = (*state)[1][3];
  (*state)[1][3] = temp;
}

void InvShiftRows(state_t *state) {
  uint8_t temp;

  // Rotate first row 1 columns to right
  temp = (*state)[3][1];
  (*state)[3][1] = (*state)[2][1];
  (*state)[2][1] = (*state)[1][1];
  (*state)[1][1] = (*state)[0][1];
  (*state)[0][1] = temp;

  // Rotate second row 2 columns to right
  temp = (*state)[0][2];
  (*state)[0][2] = (*state)[2][2];
  (*state)[2][2] = temp;

  temp = (*state)[1][2];
  (*state)[1][2] = (*state)[3][2];
  (*state)[3][2] = temp;

  // Rotate third row 3 columns to right
  temp = (*state)[0][3];
  (*state)[0][3] = (*state)[1][3];
  (*state)[1][3] = (*state)[2][3];
  (*state)[2][3] = (*state)[3][3];
  (*state)[3][3] = temp;
}

void MixColumns(state_t *state) {
  uint8_t i;
  uint8_t Tmp, Tm, t;
  for (i = 0; i < 4; ++i) {
    t = (*state)[i][0];
    Tmp = (*state)[i][0] ^ (*state)[i][1] ^ (*state)[i][2] ^ (*state)[i][3];
    Tm = (*state)[i][0] ^ (*state)[i][1];
    Tm = xtime(Tm);
    (*state)[i][0] ^= Tm ^ Tmp;
    Tm = (*state)[i][1] ^ (*state)[i][2];
    Tm = xtime(Tm);
    (*state)[i][1] ^= Tm ^ Tmp;
    Tm = (*state)[i][2] ^ (*state)[i][3];
    Tm = xtime(Tm);
    (*state)[i][2] ^= Tm ^ Tmp;
    Tm = (*state)[i][3] ^ t;
    Tm = xtime(Tm);
    (*state)[i][3] ^= Tm ^ Tmp;
  }
}

void InvMixColumns(state_t *state) {
  int i;
  uint8_t a, b, c, d;
  for (i = 0; i < 4; ++i) {
    a = (*state)[i][0];
    b = (*state)[i][1];
    c = (*state)[i][2];
    d = (*state)[i][3];

    (*state)[i][0] = multiply(a, 0x0e) ^ multiply(b, 0x0b) ^ multiply(c, 0x0d) ^
                     multiply(d, 0x09);
    (*state)[i][1] = multiply(a, 0x09) ^ multiply(b, 0x0e) ^ multiply(c, 0x0b) ^
                     multiply(d, 0x0d);
    (*state)[i][2] = multiply(a, 0x0d) ^ multiply(b, 0x09) ^ multiply(c, 0x0e) ^
                     multiply(d, 0x0b);
    (*state)[i][3] = multiply(a, 0x0b) ^ multiply(b, 0x0d) ^ multiply(c, 0x09) ^
                     multiply(d, 0x0e);
  }
}

void AddRoundKey(uint8_t round, state_t *state, const uint8_t *RoundKey) {
  uint8_t i, j;
  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      (*state)[j][i] ^= RoundKey[(round * Nb * 4) + (i * Nb) + j];
}

void AES_Encrypt(const uint8_t *in, uint8_t *out, const uint8_t *RoundKey) {
  state_t state;
  uint8_t round = 0;

  // Copy input block to state
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      state[j][i] = in[i * 4 + j];

  // Initial round key addition
  AddRoundKey(round, &state, RoundKey);

  for (round = 1; round < Nr; ++round) {
    SubBytes(&state);
    ShiftRows(&state);
    MixColumns(&state);
    AddRoundKey(round, &state, RoundKey);
  }

  SubBytes(&state);
  ShiftRows(&state);
  AddRoundKey(Nr, &state, RoundKey);

  // Copy state to output block
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      out[i * 4 + j] = state[j][i];
}

void AES_Decrypt(const uint8_t *in, uint8_t *out, const uint8_t *RoundKey) {
  state_t state;
  uint8_t round = Nr;

  // Copy input block to state
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      state[j][i] = in[i * 4 + j];

  // Initial round key addition
  AddRoundKey(round, &state, RoundKey);

  for (round = Nr - 1; round > 0; --round) {
    InvShiftRows(&state);
    InvSubBytes(&state);
    AddRoundKey(round, &state, RoundKey);
    InvMixColumns(&state);
  }

  InvShiftRows(&state);
  InvSubBytes(&state);
  AddRoundKey(0, &state, RoundKey);

  // Copy state to output block
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      out[i * 4 + j] = state[j][i];
}
