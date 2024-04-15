.section .data
.section .text

.global hamming_dist

# Hamming distance calculation between two strings
# Inputs:
#   rdi: pointer to the first string
#   rsi: pointer to the second string
# Outputs:
#   rax: Hamming distance (64 - count of differing bytes)

hamming_dist:
    pushq %rbp               # Save current base pointer
    movq %rsp, %rbp          # Set up a new base pointer
    
    # set rax to 0
    xorq  %rax, %rax
    xorq  %rdx, %rdx
    xorq  %rcx, %rcx
    
    ###################################################################################### PART 1
    
    # Load the first 64 bytes of each input string into zmm registers.
    vmovdqu64 (%rdi), %zmm0
    vmovdqu64 (%rsi), %zmm1

    # Perform bitwise XOR operation between %zmm0 and %zmm1
    vpxorq %zmm0, %zmm1, %zmm0

    # Clear %zmm1 to use it as a mask for comparison
    vpxorq %zmm1, %zmm1, %zmm1

    # Set %zmm0 to 0xFF where bytes in %zmm0 are zero, If the bytes are non-zero, they will be set to 0x00 in those positions. Store the result in k1 register.
    vpcmpeqb %zmm1, %zmm0, %k1

    # Move each byte in %zmm0 to a 64-bit integer mask, still checking the MSB for 0 but now it will have -1 (0xFF) if the chars are different.
    kmovq %k1, %rcx

    # Count the number of set bits in %rcx (number of zero bytes)
    popcnt %rcx, %rcx

    # Calculate 64 - count because we calculated the opposite of what we needed. 64 bytes are in zmm, so now check for (64 - the count), and the result is the difference.
    movq $64, %rdx            # Set %rdx to 64
    subq %rcx, %rdx           # Subtract the count from 64
    addq %rdx, %rax           # Store the result in %rax
    
    ###################################################################################### PART 2
    
    # Load the first 64 bytes of each input string into zmm registers.
    vmovdqu64 64(%rdi), %zmm0
    vmovdqu64 64(%rsi), %zmm1

    # Perform bitwise XOR operation between %zmm0 and %zmm1.
    vpxorq %zmm0, %zmm1, %zmm0

    # Clear %zmm1 to use it as a mask for comparison.
    vpxorq %zmm1, %zmm1, %zmm1

    # Set %zmm0 to 0xFF where bytes in %zmm0 are zero, If the bytes are non-zero, they will be set to 0x00 in those positions. Store the result in k1 register.
    vpcmpeqb %zmm1, %zmm0, %k1

    # Move each byte in %zmm0 to a 64-bit integer mask, still checking the MSB for 0 but now it will have -1 (0xFF) if the chars are different.
    kmovq %k1, %rcx

    # Count the number of set bits in %rcx (number of zero bytes).
    popcnt %rcx, %rcx

    # Calculate 64 - count because we calculated the opposite of what we needed. 64 bytes are in zmm, so now check for (64 - the count), and the result is the difference.
    movq $64, %rdx            # Set %rdx to 64
    subq %rcx, %rdx           # Subtract the count from 64
    addq %rdx, %rax           # Store the result in %rax
    
    
    ###################################################################################### PART 3
    
    # Load the first 64 bytes of each input string into zmm registers.
    vmovdqu64 128(%rdi), %zmm0
    vmovdqu64 128(%rsi), %zmm1

    # Perform bitwise XOR operation between %zmm0 and %zmm1
    vpxorq %zmm0, %zmm1, %zmm0

    # Clear %zmm1 to use it as a mask for comparison
    vpxorq %zmm1, %zmm1, %zmm1

    # Set %zmm0 to 0xFF where bytes in %zmm0 are zero, If the bytes are non-zero, they will be set to 0x00 in those positions.
    vpcmpeqb %zmm1, %zmm0, %k1

    # Move each byte in %zmm0 to a 64-bit integer mask, still checking the MSB for 0 but now it will have -1 (0xFF) if the chars are different.
    kmovq %k1, %rcx

    # Count the number of set bits in %rcx (number of zero bytes)
    popcnt %rcx, %rcx

    # Calculate 64 - count because we calculated the opposite of what we needed. 64 bytes are in zmm, so now check for (64 - the count), and the result is the difference.
    movq $64, %rdx            # Set %rdx to 64
    subq %rcx, %rdx           # Subtract the count from 64
    addq %rdx, %rax           # Store the result in %rax


    ###################################################################################### PART 4
    
    # Load the first 64 bytes of each input string into zmm registers
    vmovdqu64 192(%rdi), %zmm0
    vmovdqu64 192(%rsi), %zmm1

    # Perform bitwise XOR operation between %zmm0 and %zmm1
    vpxorq %zmm0, %zmm1, %zmm0

    # Clear %zmm1 to use it as a mask for comparison
    vpxorq %zmm1, %zmm1, %zmm1

    # Set %zmm0 to 0xFF where bytes in %zmm0 are zero, If the bytes are non-zero, they will be set to 0x00 in those positions.
    vpcmpeqb %zmm1, %zmm0, %k1

    # Move each byte in %zmm0 to a 64-bit integer mask, still checking the MSB for 0 but now it will have -1 (0xFF) if the chars are different.
    kmovq %k1, %rcx

    # Count the number of set bits in %rcx (number of zero bytes)
    popcnt %rcx, %rcx

    # Calculate 64 - count because we calculated the opposite of what we needed. 64 bytes are in zmm, so now check for (64 - the count), and the result is the difference.
    movq $64, %rdx            # Set %rdx to 64
    subq %rcx, %rdx           # Subtract the count from 64
    addq %rdx, %rax           # Store the result in %rax
    
    
    movq %rbp, %rsp          # Restore the previous stack frame
    popq %rbp                # Restore the previous base pointer
    ret                      # Return from the function
