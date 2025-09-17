**一、单调队列解题详解**

[239. 滑动窗口最大值](https://leetcode.cn/problems/sliding-window-maximum/)

这道题不复杂，难点在于如何在 O(1) 时间算出每个「窗口」中的最大值，使得整个算法在线性时间完成。在之前的文章【一个通用思想解决股票问题】中我们探讨过类似的场景，得到一个结论：

在一堆数字中，已知最值，如果给这堆数添加一个数，那么比较一下就可以很快算出最值；但如果减少一个数，就不一定能很快得到最值了，而要遍历所有数重新找最值。

回到这道题的场景，每个窗口前进的时候，要添加一个数同时减少一个数，所以想在 O(1) 的时间得出新的最值，就需要「单调队列」这种特殊的数据结构来辅助了。

滑动窗口解法链接：https://leetcode.cn/problems/find-all-anagrams-in-a-string/solutions/9749/hua-dong-chuang-kou-tong-yong-si-xiang-jie-jue-zi-

739. 每日温度
      递减栈

二、技巧
青蛙跳台阶拓展：一只青蛙一次可以跳上1级台阶，也可以跳上2级……它也可以跳上n级。求该青蛙跳上一个n级的台阶(n为正整数)总共有多少种跳法。
f(n)=f(n-1)+f(n-2)+……+f(2)+f(1);
f(n-1)=f(n-2)+……+f(2)+f(1);
f(n)=2*f(n-1)


异或：
相同为0，不同为1，即
1 ^ 1 = 0
0 ^ 0 = 0
1 ^ 0 = 1
由运算规则可知，任何二进制数与零异或，都会等于其本身，即 A ^ 0 = A。
异或性质
（1）交换律： A ^ B = B ^ A
（2）结合律： ( A ^ B ) ^ C = A ^ ( B ^ C )
（3）自反性： A ^ B ^ B = A （由结合律可推： A ^ B ^ B = A ^ ( B ^ B ) = A ^ 0 = A）


31、下一个排列
https://leetcode.cn/problems/next-permutation/solutions/80560/xia-yi-ge-pai-lie-suan-fa-xiang-jie-si-lu-tui-dao-/?envType=study-plan-v2&envId=top-100-liked
思路：从数组最后找到最前第一个升序的点i，交换i+1,end中第一个大于nums[i]的点nums[j],这个点一定存在，除非数组是降序的，如果数组降序的，那么i=-1，


三、和为k的子数组
除了暴力法，可以采用前缀和+哈希的方法，即303和1、两数之和题解法的组合
然后两种情况都可以使用reverse(nums.begin()+i+1, nums.end());


三、买卖股票的最佳时机：
参考：
https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-iv/solutions/740596/5xing-dai-ma-gao-ding-suo-you-gu-piao-ma-j6zo/
