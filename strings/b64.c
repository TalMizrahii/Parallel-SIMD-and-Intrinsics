#include <immintrin.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "libstr.h"

/**
 * Modify a a string of base 64 chars to a their represented numbers.
 */
unsigned int modifyString(__m512i iterStr, int sum) {
    // Create a char array to store the full register.
    char charArray[65] = {0};

    // Store the vector into the char array.
    _mm512_storeu_si512(charArray, iterStr);
    int size = strlen(charArray);
    // If the string is empty, return.
    if(!size){
        return sum;
    }

    /**
    * Handle the '+' = 43 to 62 by adding 19.
    */
    // Define a mask for bytes equal to 43
    __mmask64 mask = _mm512_cmpeq_epi8_mask(iterStr, _mm512_set1_epi8(43));
    // Add 19 only to bytes equal to 43.
    iterStr = _mm512_mask_add_epi8(iterStr, mask, iterStr, _mm512_set1_epi8(19));

    /**
    * Handle the '+' = 47 to 63 by adding 16.
    */
    // Define a mask for bytes equal to 47.
    mask = _mm512_cmpeq_epi8_mask(iterStr, _mm512_set1_epi8(47));
    // Add 16 only to bytes equal to 47.
    iterStr = _mm512_mask_add_epi8(iterStr, mask, iterStr, _mm512_set1_epi8(16));

    /**
    * Numbers update.
    */
    mask = _mm512_cmpge_epi8_mask(iterStr, _mm512_set1_epi8('0')) &
            _mm512_cmple_epi8_mask(iterStr, _mm512_set1_epi8('9'));
    // Add 4 only from the numbers.
    iterStr = _mm512_mask_add_epi8(iterStr, mask, iterStr, _mm512_set1_epi8(4));

    /**
    *   Lowercase update.
    */
    mask = _mm512_cmpge_epi8_mask(iterStr, _mm512_set1_epi8('a')) &
            _mm512_cmple_epi8_mask(iterStr, _mm512_set1_epi8('z'));
    // Subtract 71 only from the lower case letters.
    iterStr = _mm512_mask_sub_epi8(iterStr, mask, iterStr, _mm512_set1_epi8(71));

    /**
     * Uppercase update.
     */
    mask = _mm512_cmpge_epi8_mask(iterStr, _mm512_set1_epi8('A')) &
            _mm512_cmple_epi8_mask(iterStr, _mm512_set1_epi8('Z'));
    // Subtract 65 only from the upercase letters.
    iterStr = _mm512_mask_sub_epi8(iterStr, mask, iterStr, _mm512_set1_epi8(65));


    // Store the vector into the char array
    _mm512_storeu_si512(charArray, iterStr);

    // Calculate the sum by the algorithm.
    for (int i = 0; i < size; i++) {
        sum = sum * 64 + (int)charArray[i];
    }
    return sum;
}

int b64_distance(char str1[MAX_STR], char str2[MAX_STR]) {

    // Intialize the accumulator masks and the result masks.
    __mmask64 finalMask1, validCharMask1, finalMask2, validCharMask2;

    // define the charMask and the 512 bit registers to put the original 64 byte part string in.
    __m512i charMask, iterStr1, iterStr2;

    // Set the result for the two strings values in base 64.
    int baseSum1 = 0, baseSum2 = 0;

    // 256 bytes for MAX_STR, so 64 at a run, so 4 times. The loop removes the unwanted chars.
    for(int i = 0; i < 4; i++){

        // Load 64 characters from each string into 512-bit registers.
        iterStr1 = _mm512_loadu_si512((__m512i*)&str1[64*i]);
        iterStr2 = _mm512_loadu_si512((__m512i*)&str2[64*i]);

        // Filter in the lowercase letters.
        finalMask1 = _mm512_cmpge_epi8_mask(iterStr1, _mm512_set1_epi8('a')) &
                         _mm512_cmple_epi8_mask(iterStr1, _mm512_set1_epi8('z'));

        finalMask2 = _mm512_cmpge_epi8_mask(iterStr2, _mm512_set1_epi8('a')) &
                         _mm512_cmple_epi8_mask(iterStr2, _mm512_set1_epi8('z'));

        // Filter in the uppercase letters.
        validCharMask1 = _mm512_cmpge_epi8_mask(iterStr1, _mm512_set1_epi8('A')) &
                         _mm512_cmple_epi8_mask(iterStr1, _mm512_set1_epi8('Z'));

        validCharMask2 = _mm512_cmpge_epi8_mask(iterStr2, _mm512_set1_epi8('A')) &
                         _mm512_cmple_epi8_mask(iterStr2, _mm512_set1_epi8('Z'));

        finalMask1 |= validCharMask1;
        finalMask2 |= validCharMask2;

        // Filter in the numbers.
        validCharMask1 = _mm512_cmpge_epi8_mask(iterStr1, _mm512_set1_epi8('0')) &
                         _mm512_cmple_epi8_mask(iterStr1, _mm512_set1_epi8('9'));

        validCharMask2 = _mm512_cmpge_epi8_mask(iterStr2, _mm512_set1_epi8('0')) &
                         _mm512_cmple_epi8_mask(iterStr2, _mm512_set1_epi8('9'));

        finalMask1 |= validCharMask1;
        finalMask2 |= validCharMask2;

        // Set the char to keep!
        charMask= _mm512_set1_epi8('/');

        // Create a mask for the current valid character.
        validCharMask1 = _mm512_cmpeq_epi8_mask(iterStr1, charMask);
        validCharMask2 = _mm512_cmpeq_epi8_mask(iterStr2, charMask);

        // OR the current validCharMask with the finalMask.
        finalMask1 |= validCharMask1;
        finalMask2 |= validCharMask2;

        // Set the char to keep!
        charMask= _mm512_set1_epi8('+');

        // Create a mask for the current valid character.
        validCharMask1 = _mm512_cmpeq_epi8_mask(iterStr1, charMask);
        validCharMask2 = _mm512_cmpeq_epi8_mask(iterStr2, charMask);

        // OR the current validCharMask with the finalMask.
        finalMask1 |= validCharMask1;
        finalMask2 |= validCharMask2;

        // Compress the input strings using the valid character masks.
        iterStr1 = _mm512_mask_compress_epi8(_mm512_setzero_si512(), finalMask1, iterStr1);
        iterStr2 = _mm512_mask_compress_epi8(_mm512_setzero_si512(), finalMask2, iterStr2);

        // Sum the result.
        baseSum1 = modifyString(iterStr1, baseSum1);
        baseSum2 = modifyString(iterStr2, baseSum2);
    }

    // Return the subtruction of string 1 from string 2.
    return baseSum2 - baseSum1;
}