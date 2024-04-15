.section .text
.global formula2

formula2:
    pushq %rbp
    movq %rsp, %rbp

    movq %rdx, %r13         # Save the address of the x vector
    # Assuming %rdx holds the total number of elements (n)
    movq %rdx, %rax         # Copy n to rax to prepare for division
    xor %rdx, %rdx          # Clear rdx to use it as the high 64 bits of the dividend

    # The divisor is 4 (to calculate n/4 and n%4)
    movq $4, %rcx           # Move the divisor (4) into rcx for divq

    divq %rcx               # Divide rdx:rax by rcx, quotient -> rax, remainder -> rdx

    # At this point:
    # rax now contains the number of full blocks (n / 4)
    # rdx now contains the remainder (n % 4)

    # Move the results to %r11 (number of full blocks) and %r10 (remainder)
    movq %rax, %r11         # Move quotient (number of full blocks) to r11
    movq %rdx, %r10         # Move remainder to r10


    xorq %r8, %r8                   # Reset index for vector elements
    movq %rdx, %rcx                 # Move n (vector length) into %rcx for loop counter

    # Initialize numerator and denominator
    # make xmm8 to zero
    xorps %xmm8, %xmm8      # Set xmm8 to zero, numerator

    mov $0x3F800000, %eax          # Single-precision representation of 1.0
    vmovd %eax, %xmm0              # Move the value into the lower 32 bits of an XMM register
    vbroadcastss %xmm0, %xmm9      # Broadcast 1.0 to all elements in xmm9



loop_start:
    vmovups (%rdi, %r8), %xmm3      # Load x vector elements into XMM3
    vmovups (%rsi, %r8), %xmm4      # Load y vector elements into XMM4

    # zero all the xmm0, xmm1, xmm2
    xorps %xmm0, %xmm0
    xorps %xmm1, %xmm1
    xorps %xmm2, %xmm2


    # Calculate the numerator (sum of products)
    vmulps %xmm3, %xmm4, %xmm0      # Calculate x_k * y_k
    vaddps %xmm8, %xmm0, %xmm8      # Accumulate the numerator



    # Prepare and calculate each term for the denominator
    vsubps %xmm3, %xmm4, %xmm2      # calculate x - y
    vmulps %xmm2, %xmm2, %xmm2      # Calculate (x - y)^2

    # add 1.0 to all elements in xmm2
    mov $0x3F800000, %eax          # Single-precision representation of 1.0
    vmovd %eax, %xmm0              # Move the value into the lower 32 bits of an XMM register
    vbroadcastss %xmm0, %xmm0      # Broadcast 1.0 to all elements in xmm0
    vaddps %xmm2, %xmm0, %xmm2     # Resulting in (x - y)^2 + 1 in xmm2

    vmulps %xmm9, %xmm2, %xmm9      # Accumulate the denominator



    addq $16, %r8                   # Adjust index for next iteration
    subq $1, %r11                   # Decrement loop counter
    jnz loop_start                  # Continue if not done


scalar_loop_start:
    testq %r10, %r10                # Check if there are any remaining elements
    jz end                          # If not, jump to the end

    movss (%rdi, %r8, 1), %xmm1     # Load next element of x into xmm1
    movss (%rsi, %r8, 1), %xmm2     # Load next element of y into xmm2

    # Calculate the numerator for the scalar value
    movaps %xmm1, %xmm0             # Copy x_k to xmm0
    mulss %xmm2, %xmm0              # xmm0 = x_k * y_k for the current element
    addss %xmm0, %xmm8              # Add the result to the first element of xmm8 (A += x_k * y_k)

    # Calculate (x_k - y_k)^2 + 1 for the scalar value
    movaps %xmm1, %xmm0             # Copy x_k to xmm0
    subss %xmm2, %xmm0              # xmm0 = x_k - y_k
    mulss %xmm0, %xmm0              # xmm0 = (x_k - y_k)^2
    addss %xmm0, %xmm9              # Add the result to the first element of xmm9, assuming xmm9 setup for denominator

    # Increment index and decrement remainder
    addq $4, %r8                    # Move to the next element
    decq %r10                       # Decrement the count of remaining elements
    jnz scalar_loop_start           # If there are more elements, continue

    jmp end                         # Jump to the end when done
         

end:
    # zero all the xmm3, xmm4, xmm0
    xorps %xmm0, %xmm0
    xorps %xmm1, %xmm1
    xorps %xmm3, %xmm3
    xorps %xmm4, %xmm4


    movaps %xmm9, %xmm0           # Copy [A, B, C, D] to xmm0
    shufps $0b10110001, %xmm9, %xmm9 # Shuffle to [B, A, D, C] in xmm9
    mulps %xmm0, %xmm9            # Now xmm9 = [A*B, B*A, C*D, D*C]

    movaps %xmm9, %xmm0           # Copy [A*B, A*B, C*D, C*D] to xmm0
    shufps $0b01001110, %xmm9, %xmm9 # Shuffle to [C*D, C*D, A*B, A*B] in xmm9

    mulps %xmm0, %xmm9            # xmm9 = [A*B*C*D, A*B*C*D, A*B*C*D, A*B*C*D]

    # move xmm9 to xmm3
    movaps %xmm9, %xmm3

    # Perform horizontal addition
    haddps %xmm8, %xmm8  # Now %xmm1 = [A+B, C+D, A+B, C+D]

    # Perform horizontal addition again to sum all
    haddps %xmm8, %xmm8  # Now %xmm1 = [A+B+C+D, A+B+C+D, A+B+C+D, A+B+C+D]

    divps %xmm3, %xmm8  # %xmm1 = [(A+B+C+D)/(A*B*C*D), ..., (A+B+C+D)/(A*B*C*D)]

    vmovups %xmm8, %xmm0  # Move the result to xmm0

    movq %rbp, %rsp
    popq %rbp
    ret
