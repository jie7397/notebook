# 左神第一课：

## ^ 异或操作。
异或操作可以认为是无进位相加。  
基本性质：  
```c++
a ^ 0 = a
a ^ a = 0
// 符合交换律
a ^ b ^ c = a ^ (b ^ c)
```
所以交换a, b数据可以写成   
``` c++
a = a ^ b
b = a ^ b 
a = a ^ b
```

`^`操作用法：  
1）数组arr1中每个元素都为int，且只有一种数出现了奇数次。找出该数。  
```c++
int solution(int [] arr) {
    int eor = 0;
    for (int cur : arr) {
        eor ^= cur;
    }
    std::cout << eor << std::endl;
}
```
2）数组arr1中每个元素都为int，且只有两种数出现了奇数次。找出这两个数。  
```c++
int solution(int [] arr) {
    int eor = 0, onlyOne = 0;
    for (int cur : arr) {
        eor ^= cur;
    }
    std::cout << eor << std::endl;

    // 取eor二进制码，最右侧第一个1的位置。
    int rightone = eor & (~eor + 1);

    for(int cur : arr) {
        if ((cur & rightOne) == 0) {
            onlyOne ^=cur;
        }
    }

    eor ^= onlyOne;
    std::cout << eor << std::endl;
    std::cout << onlyOne << std::endl;
}
```

arr无序，相邻数一定不相等。   
局部最小：arr[0] < 1; arr[n-1] < arr[n-2]; arr[i-1] < arr[i] & arr[i+1] < arr[1]  
解法： **二分法**

对数器:
1.有一个你想要测的方法a，实现复杂度不好但是容易实现的方法b，实现随机样本生成器，用方法a和方法b跑同样的样本


# 左神第二课：
```c++
// arr[L..R]范围上的最大值
int process(int[] arr, int L, int R) {
    if (L == R) {
        return arr[L];
    }
    // 防止L+R溢出，=> L+(R-L)/2 => L+(R-L)>>2  
    int mid = L + ((R -L) >> 1); 
    int letMax = process(arr, L, mid);
    int rightMax = process(arr, mid + 1, R);
    return std::max(letMax, rightMax)
}
```

**master公式：`T(N) = a * T(N/b) + O(N^d)`**
* `T(N)`: 母问题有N个数据
* `a`: 子问题被调用多少次
* `T(N/b)` : 子问题的规模**都是**N/b
* `O(N^d)`:除了子问题其他的程序的时间复杂度
  
`arr[L..R]`范围上的最大值解法,带入master公式：`T(N) = 2 * T(N/2) + O(1)`  
1） log(a,b) > d -> 复杂度：O(N^log(b,a))  
2） log(a,b) = d -> 复杂度：O(N^d * logN)  
3） log(a,b) < d -> 复杂度：O(N^d)  

## 归并排序
```c++
int process(int[] arr, int L, int R) {

}
```