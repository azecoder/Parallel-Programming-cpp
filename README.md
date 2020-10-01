# Parallel Programming - Cpp

Task (Odd-Even Sort): Odd even sort works exchanging adjacent, out-of-order items in the vector to be sorted. Sorting proceeds in iterations. At each iteration, first each even position item is compared with the following (odd) one, and, in case they are out of order, they are swapped; then the odd position items are compared with the following (even position) ones and swapped if they happen to be out of order. Iteraitions go on up to the point no more swaps took place.

### Sequential Code
`Ind starts from 0 for even, from 1 for odd positions.`

`Let's assume that there is an array like [0, 1] and we run following code in this loop. But it can not be parallel loop.`

```
while (ind < len - 1) {
    if (inputArr[ind] > inputArr[ind + 1]) {
        std::swap(inputArr[ind], inputArr[ind + 1]);
        is_sorted = false;
    }
    ind += 2;
}
```



### Parallel Code

