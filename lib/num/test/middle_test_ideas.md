# Schönhage-Strassen Multiplication "Middle Test" Ideas

Here is a list of targeted "middle tests" to investigate the internal functions of `num_mul_ssm`. These tests verify the fundamental mathematical properties that the algorithm relies on. A failure here will point much more directly to the bug.

### 1. Test Linearity of the Forward Transform

**Why it's useful:** The Fast Fourier Transform (and your implementation of it) must be a linear transform. This means that `fwd(A) + fwd(B) = fwd(A + B)`. If this property doesn't hold, the transform will not correctly decompose the numbers for pointwise multiplication, and the final result will be wrong. This is a more fundamental check than the roundtrip test.

**How to test it:**
1.  Create two random `num_p` numbers, `A` and `B`, of the same size.
2.  Compute `C = A + B`.
3.  Transform each number individually: `fwd_A = num_mul_ssm_fwd_transform(A)`, `fwd_B = num_mul_ssm_fwd_transform(B)`.
4.  Transform their sum: `fwd_C = num_mul_ssm_fwd_transform(C)`.
5.  Add the transformed results together element-by-element (pointwise): `fwd_A_plus_B = fwd_A + fwd_B`. This addition must be performed with the same modular arithmetic used inside the SSM algorithm.
6.  **Assert that `fwd_C` is equal to `fwd_A_plus_B`.** If they are not equal, your forward transform is not linear, which is a critical bug.

### 2. Test the Pointwise Multiplication Step

**Why it's useful:** The core of SSM is that convolution in the time domain becomes simple pointwise multiplication in the frequency domain. Your existing fuzz test for `num_mul_ssm` versus `num_mul_classic` fails, and you suspect the pointwise multiplication. This test will help you isolate it. It's likely that this multiplication is modular, e.g., `(a * b) mod M`. A bug in this modular multiplication logic would completely invalidate the SSM result.

**How to test it:**
1.  Isolate the function or code segment that performs the element-by-element multiplication of the transformed numbers. Let's assume it's equivalent to a function `pointwise_mul(fwd_A, fwd_B)`.
2.  Create two small, known `num_p` numbers, `A` and `B`.
3.  Manually calculate the expected product `C = A * B` using `num_mul_classic`.
4.  Perform the forward transform on `A` and `B` to get `fwd_A` and `fwd_B`.
5.  Perform the pointwise multiplication: `fwd_C_pointwise = pointwise_mul(fwd_A, fwd_B)`.
6.  Perform the forward transform on the classic result: `fwd_C_classic = fwd_transform(C)`.
7.  **Assert that `fwd_C_pointwise` is equal to `fwd_C_classic`**. This directly verifies the convolution theorem. If this fails, the issue is almost certainly in the pointwise multiplication logic itself.

### 3. Verify the `num_mul_ssm_finish` function

**Why it's useful:** After the inverse transform, you get a sequence of coefficients that need to be recombined with carries to form the final, large integer. The `num_mul_ssm_finish` function likely does this. An error here would mean that even with a perfect transform and pointwise multiplication, the final result is assembled incorrectly.

**How to test it:**
1.  Construct a synthetic, known input for `num_mul_ssm_finish`. This input represents the coefficients of the result polynomial before carry propagation. For example, create a `num_p` that represents `[0, 1, 1, 0, ...]` which corresponds to `x^2 + x`.
2.  Manually calculate what the final integer value should be. For `x^2 + x` where `x` is the base of your chunks (e.g., `2^64`), the result would be `(2^64)^2 + 2^64`.
3.  Call `num_mul_ssm_finish` with your synthetic input.
4.  **Assert that the output of the function matches your manually calculated result.** You can try this with several simple, known inputs to ensure the carry propagation logic is sound.
