姓名：冯运
学号：1160300524
一、
1. $R_1$和$R_2$是两个关系下列结论中错误的是哪一个，为什么？
A. $\Pi_F(R_1\cap R_2)=\Pi_F(R_1)\cap\Pi_F(R_2)$
B. $R_1\cup R_2=R_2\cup R_1$
C. $R_1\cap R_2\cap R_3=R_1\cap (R_2\cap R_3)$
D. $\Pi_F(R_1 \cup R_2)=\Pi_F(R_1)\cup \Pi_F(R_2)$
选择A选项
因为$\Pi(R_1\cap R_2)$是先做交运算，后做选择操作,例如
R1:

| F   | B   |
| --- | --- |
| a   | 1   |
与R2:
| F   | B   |
| --- | --- |
| a   | 2   |
$\Pi(R_1\cap R_2)$得到的就是空关系，而$\Pi_F(R_1)\cap\Pi_F(R_2)$得到的就是
|F|
|-|
|a|
所以两个是不相同的
2. 设R和S的属性个数分别是r和s，那么R $^\Join_{i\theta j}$ S与下式哪一个等价(i和j是R第i和S第j个属性)，为什么？

A. $\delta _{i\theta (r+j)}$(R x S)

B. $\delta _{i\theta j}$(R x S)

C. $\delta _{i\theta (r+j)}$(R $\Join$ S)

D. $\delta _{i\theta j}$(R $\Join$ S)
B
根据$\theta$连接的定义，应该选择B,即R与R的笛卡尔乘积中满足$i\theta j$表达式的集合
二、有如下四个关系
厂商 S(SNO,SNAME,STATUS,CITY)
产品 P(PNO,PNAME,WEIGHT,COLOR)
工程 J(JNO,JNAME,CITY)
供货 SPJ(SNO,PNO,JNO,QTY)

其中,厂商表 S 由厂商号(SNO)、厂商名(SNAME)、厂商状态(STATUS)、厂商所在城市(CITY)组成、产品表 P 由产品号(PNO)、产品名(PNAME)、重量(WEIGHT)、颜色(COLOR)组成;
工程表 J 由工程号(JNO)、工程名(JNAME)、工程所在城市(CITY)组成;
供货表 SPJ 由供货厂商号(SNO)、产品号(PNO)、工程号(JNO)、供货数量(QTY)组成。
用关系代数写出下述操作.
1. 给出由 LODON(伦敦)的厂商供给 LODON 的工程的产品号。
   $$\Pi_{PNO}(\sigma_{CITY="LODON"}(S)\Join SPJ\Join\sigma_{CITY="LODON"}(J))$$
2. 给出满足如下条件的所有产品号:提供该零件的厂商和使用该零件的工程在同一城市。
   $$\Pi_{PNO}(S\Join J\Join SPJ)$$
3. 给出由 S1(厂家号)提供产品的工程名。
   $$\Pi_{JNO}(\sigma_{SNO=S1}(SPJ))$$
4. 给出使用了由供应红色产品的厂商供应的产品的工程名.
   $$\Pi_{JNO}(\Pi_{SNO}(sigma_{COLOR=RED}(P)\Join SPJ)\Join SPJ)$$
5. 求使用了全部零件的工程名。
   $$\Pi_{PNO,JNO}(SPJ)\div\Pi_{PNO}(P)$$
6. 同时供应 P1 和 P2(产品号)两种产品的厂家名。
   $$(\Pi_{SNAME}(\sigma_{PNAME="P1"}(SPJ)))\cap(\Pi_{SNAME}(\sigma_{PNAME="P2"}(SPJ)))$$
7. 求与 “TV”(产品名)颜色相同的那些产品的产品名。
   $$\Pi_{PNAME}(\Pi_{COLOR}(\sigma_{PNAME="TV"}(P))\Join P)$$

三、设有关系R(A, B, C)和A(A, B, C),请把下列关系代数表达式转换成等价的元组表达式。
(1)$\pi _{B, C}$(R)
$$\{t|\exists u\in R(t[B]=u[B]\wedge t[C]=u[C])\}$$
(2)$\sigma _{B > C}$(R)
$$\{t|t\in R \wedge t[B]>t[C]\}$$
(3)$R \cup S$
$$\{t|t\in R \vee t\in S\}$$
(4)$R \cap S$
$$\{t|t\in R \wedge t\in S\}$$
(5)$R-S$
$$\{t|\in R \wedge t\notin S\}$$
(6)$\pi_{A,B}(R)\Join\pi_{B,C}(S)$
$$\{t|\exists u\in R(u[A]=t[A]\wedge u[B]=t[B])\wedge \exists v\in S(v[B]=t[B]\wedge v[C]=t[C])\}$$
(7)$R\div\pi_C(S)$
先定义：$$T=\{t|\exists u\in S(u[C]=t[C])\}$$
$R\div\pi_C(S)$=
$$\{t|\exists u\in R(u[A]=t[A]\wedge u[B]=t[B])\wedge\forall v\in T(tv\in R)\}$$
四、谈一谈关系代数、元组演算和域演算的联系和区别。
联系：
通过关系代数，元组演算和域演算都可以准确的通过形式化的方式描述某种特定的查询，它们是相互等价的，即某种查询都可以通过三种方式来表达，得到的结果是相同的。
区别：
关系代数表达式提供了产生查询结果的过程序列小，注重描述查询过程方法，而元组演算和域演算只描述所需的信息，不给出获取该信息的具体过程，注重表达结果的特征。
而元组演算与域演算的区别在于元组演算的描述单位是元组，而域演算的描述单位是域变量，即属性名。