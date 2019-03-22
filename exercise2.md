姓名：冯运
学号：1160300524
一、在学生管理数据库中,有如下三个表
学生信息表:S(S#,SNAME, AGE, SEX),
课程表:C(C#,CNAME,TEACHER)
选课表:SC(S#, C#, GRADE)。
请使用 SQL 回答下列问题:
(1) 检索学号为 003 号同学所学课程的课程名和教师名
```
SELECT CNAME, TEACHER
FROM C,SC
WHERE SC.S#='003'and
      C.C#=SC.C#;
```
(2) 检索选择程军老师所授课程的男同学姓名
```
SELECT SNAME
FROM S,C,SC
WHERE TEACHER='程军' and
      SEX='M' and
      S.S#=SC.S# and
      C.C#=SC.C#;
```
(3) 检索刘丽同学不学的课程的课程名
```
SELECT CNAME
FROM C
WHERE C#<>ALL(
  SELECT C#
  FROM S,SC
  WHERE SNAME='刘丽' and
        S.S#=SC.S#
)
```
(4) 检索平均成绩在 60 分以下的学生的学号和姓名
```
SELECT S.S#,SNAME
FROM S,C,SC
WHERE S.S#=SC.S# and
      C.C#=SC.C#
Group by S#,SNAME having avg(GRADE)<60;
```
(5) 检索至少选修 3 门课程的学生的学号,按照从小到大排列。
```
SELECT S.S#
WHERE S.S#=SC.S# and
      C.C#=SC.C#
Group by S.S# having count(C#)>=3
Order by S.S# ASC；
```

二、涉及二战中的大型舰船。它由以下几个关系组成(红色标记主键,蓝色标记外键):
Classes(<font color="red">class</font>, type, country, numGuns, bore, displacement)
Ships(<font color="red">name</font>, <font color="blue">class</font>, launched)
Battles(<font color="red">name</font>, date)
Outcomes(<font color="red">ship</font>,<font color="red">battle</font>, result)
舰船类表 Classes 由舰船类名(class)、型号(type,其中 bb 代表主力舰,bc 代表巡洋
舰)、生产国家(country)、火炮的门数(numGuns)、火炮口径(bore,单位是英寸)和排水量(displacement,单位是吨)组成。
舰船表 Ships 由舰船名(name)、舰船类名(class)、开始服役的日期(launched)组成。
战役表 Battles 由战役名(name)和舰船参加战役的时间(date)组成。
参战结果表 Outcomes 由舰船名(ship)、参加的战役名(battle)和各个舰船在各场战役中的结果(result,分为沉没、受伤或完好)组成。
(a) 关系Classes的数据取样

| class          | type | country    | numGuns | bore | displacement |
| -------------- | ---- | ---------- | ------- | ---- | ------------ |
| Bismarck       | bb   | Germany    | 8       | 15   | 42000        |
| Iowa           | bb   | USA        | 9       | 16   | 46000        |
| Kongo          | bc   | Japan      | 8       | 14   | 32000        |
| North Carolina | bb   | USA        | 9       | 16   | 37000        |
| Renown         | bc   | Gt.Britain | 6       | 15   | 32000        |
| Revenge        | bb   | Gt.Britain | 9       | 15   | 29000        |
| Tennessee      | bb   | USA        | 12      | 14   | 32000        |
| Yamato         | bb   | Japan      | 9       | 18   | 65000        |

(b) 关系Ships的数据取样

| name            | class          | launched |
| --------------- | -------------- | -------- |
| California      | Tennessee      | 1921     |
| Karuna          | Kongo          | 1915     |
| Hiei            | Kongo          | 1914     |
| Iowa            | Iowa           | 1943     |
| Kirishima       | Kongo          | 1915     |
| Kongo           | Kongo          | 1913     |
| Missouri        | Iowa           | 1944     |
| Musashi         | Yamato         | 1942     |
| New Jersey      | Iowa           | 1943     |
| North Carolina  | North Carolina | 1941     |
| Ramillies       | Revenge        | 1917     |
| Renown          | Renown         | 1916     |
| Repulse         | Renown         | 1916     |
| Resolution      | Revenge        | 1916     |
| Revenge         | Revenge        | 1916     |
| Royal Oak       | Revenge        | 1916     |
| Royal Sovereign | Revenge        | 1916     |
| Tennessee       | Tennessee      | 1920     |
| Washington      | North Carolina | 1941     |
| Wisconsin       | Iowa           | 1944     |
| Yamato          | Yamato         | 1941     |

(c) 关系Battles的数据取样

| name           | date       |
| -------------- | ---------- |
| North Atlantic | 5/24-27/41 |
| Guadalcanal    | 11/15/42   |
| North Cape     | 12/26/43   |
| Surigao Strait | 10/25/44   |

(d) 关系Outcomes的数据取样

| ship            | battle         | result  |
| --------------- | -------------- | ------- |
| Bismarck        | North Atlantic | sunk    |
| California      | Surigao Strait | ok      |
| Duke of York    | North Cape     | ok      |
| Fuso            | Surigao Strait | sunk    |
| Hood            | North Atlantic | sunk    |
| King George V   | North Atlantic | ok      |
| Kirishima       | Guadalcanal    | sunk    |
| Prince of Wales | North Atlantic | damaged |
| Rodney          | North Atlantic | ok      |
| Scharnhorst     | North Cape     | sunk    |
| South Dakota    | Guadalcanal    | damaged |
| Tennessee       | Surigao Strait | ok      |
| Washington      | Guadalcanal    | ok      |
| West Virginia   | Surigao Strait | ok      |
| Yamashiro       | Surigao Strait | sunk    |


按要求写出下面的 sql 语句:
(1) 在数据库中创建 classes 和 ships 这两个关系,并说明在插入两者的数据时,需要注意什么。
```sql
CREATE table Classes(
  class VARCHAR(20) NOT NULL,
  type	VARCHAR(2) NOT NULL,
  country VARCHAR(20) NOT NULL,
  numGuns Integer,
  bore FLOAT,
  displacement FLOAT NOT NULL,
  Primary key(class)
);
CREATE table Ships(
  name VARCHAR(20) NOT NULL,
  class VARchar(20) NOT NULL,
  launched YEAR(4) NOT NULL,
  Primary key(name),
  Foreign key(class) references classes(class)
)
```
在向Classes表插入数据时，需要注意域完整性(type，displacement不能为空，type的值只能是bb或bc)，实体完整性(class作为主键，不能为空，具有唯一性)
在向Ships表插入数据时，需要注意域完整性(launched 不能为空),实体完整性(name作为主键，不能为空，具有唯一性),关联完整性(class作为外键，保证插入的的class能在表Classes中找到)

(2) a.两艘 Nelson 类型的英国主力舰 Nelson 和 Rodney 在 1927 年下水。两者都具有 16 英寸
口径的火炮 9 门,排水量为 34000 吨。把这条信息加入到数据库中。
```sql
INSERT
INTO Classes
VALUES('Nelson','bb','Gt.Britain',9,16,34000);

INSERT
INTO Ships
VALUES('Nelson','Nelson',1927);

INSERT
INTO Ships
VALUES('Rodney','Nelson',1927);
```
b.删除所有在战役中沉没的船只。
```sql
DELETE
FROM Ships
WHERE name IN (
  SELECT ship
  FROM Outcomes
  WHERE result='sunk'
);
```
c.更新 Classes 关系使得火炮口径使用厘米作为单位(1 英寸=2.5 厘米),排水量使用公制吨(1 公制吨=1.1 吨)。
```sql
UPDATE Classes
SET bore=bore*2.5,displacement=displacement/1.1;
```
(3) 定义一个视图,它包括所有英国舰船的类型(class)、类别(type)、火炮数量、口径、排水量和下水年份。
```sql
CREATE VIEW BritishShips
AS
SELECT class,type,numGuns,bore,displacement,launched
FROM Classes,Ships
WHERE Classes.class=Ships.class and
      country='Gt.Britain';
```
(4) 查询 Kongo 类型舰船参加的战役名称。
```sql
SELECT battle
FROM Ships,Outcomes
WHERE class='Kongo' and
      Ships.name=Outcomes.ship;
```
(5) 查询所有主力舰的平均火炮数量。
```sql
SELECT avg(numGuns)
FROM Classes
Group by type Having type='bb';
```
(6) 查询每一类型的第一艘船下水的年份。
```sql
SELECT DISTINCT class,min(launched)
FROM Ships
Group by class;
```
(7) * 列出至少有 3 艘舰船的类型中,该类型名和被击沉的舰船数目。
```sql
SELECT class,SUM(CASE WHEN result='sunk' THEN 1 ELSE 0 END)
FROM Ships,Outcomes
WHERE Ships.name=Outcomes.ship
Group by class HAVING count(name)>=3;
```
(8) 列出所有数据库中提到的舰船名字。
```sql
SELECT DISTINCT name
FROM Ships;
```
(9) 列出那些既有主力舰又有巡洋舰的国家。
```sql
SELECT DISTINCT country
FROM Classes
Group by country HAVING count(DISTINCT type)=2;
```
(10) 找出拥有火炮数量最多的船只所属的国家.
```sql
SELECT DISTINCT country
FROM Classes
WHERE numGuns = ( # MAX聚集函数不能在WHERE中使用
  SELECT MAX(numGuns)
  FROM Classes
);
```
(11) 找出至少有一艘船在战役中被击没的舰船类型。
```sql
SELECT DISTINCT class
FROM Ships,Outcomes
WHERE result='sunk' and
      Ships.name=Outcomes.ship
```

三、设一个影片出租公司的数据库由以下几个关系组成：
Movies(MovieID, MovieName)
Suppliers(SupplierID, SupplierName)
Customers(CustID, LastName, FirstName)
MovieSupplier(SupplierID, MovieID, Price)
Orders(OrderID, SupplierID, MovieID, Copies)
Inventory(TapeID, MovieID)
Rentals(CustomerID, TapeID, CkoutDate, Duration)

影片表Movies由影片号（MovieID）、影片名（MovieName）组成。
供货商表Suppliers由供货商号（SupplierID）、供货商名（SupplierName）组成。
顾客表Customers由顾客号（CustID）、顾客姓氏（LastName）和顾客名（FirstName）组成。
影片供货表MovieSupplier由供货商号（SupplierID）、影片号（MovieID）和售价（Price）组成。
订单表Orders由订单号（OrderID）、供货商号（SupplierID）、影片号（MovieID）和拷贝光盘数量（Copies）组成。
库存表Inventory由光盘号（TapeID）、影片号（MovieID）组成（注意：同一部影片的多个拷贝光盘具有不同的编号）。
出租表Rentals由顾客号（CustomerID）、光盘号（TapeID）、出租日期（CkoutDate）、持续时间（Duration，单位：天）组成。

按要求写出下面的sql语句：
(1) 在数据库中创建Orders和Rentals这两个关系。
```sql
CREATE table Orders(
  OrderID VARCHAR(20),
  SupplierID VARCHAR(20),
  MovieID VARCHAR(20),
  Copies Int
);
CREATE table Rentals(
  CustomerID VARCHAR(20),
  TapeID VARCHAR(20),
  CkoutDate DATE,
  Duration int
);
```
(2) a.编号为5600的光盘被编号为9823的顾客在2017年3月26日租出，出租时长为30天。将该条信息加入到出租表Rentals中。
```sql
INSERT
INTO Rentals
VALUES('5600','9823','2017-03-26',30);
```
  b.删除出租表Rentals中所有出租日期早于2000年1月1日的记录。
  ```sql
  DELETE
  FROM Rentals
  WHERE CkoutDate<'2000-01-01';
  ```
  c.更新影片供货表MovieSupplier，使得售价从人名币改为美元作为单位（1美元=6.88人名币）。
  ```sql
  UPDATE MovieSupplier
  SET Price=Price/6.88;
  ```
(3) 定义一个视图，它包括所有由名为“Joe's House of Video”的供货商供应的影片的片名和售价。
```sql
CREATE VIEW JView
AS
SELECT MovieName,Price
FROM Movies,Suppliers,MovieSupplier
WHERE SupplierName="Joe's House of Video" and
      Movies.MovieID=MovieSupplier.MovieID and
      Suppliers.SupplierID=MovieSupplier.SupplierID;
```
(4) 列出库存中每个供货商的名称和该供货商提供的不同影片的总量。
```sql
SELECT DISTINCT SupplierName,COUNT(MovieID)
FROM Suppliers,MovieSupplier
WHERE Suppliers.supplierID=MovieSupplier.supplierID
Group by SupplierName
```
(5) 检索出租公司预订的拷贝光盘数量大于5的影片片名。
```sql
SELECT DISTINCT MovieName
FROM Movies,Orders
WHERE Movies.MovieID=Orders.MovieID
Group by MovieName HAVING SUM(Copies)>5;
```
(6) 检索在库存中的拷贝光盘数多于1的影片片名。
```sql
SELECT DISTINCT MovieName
FROM Movies,Inventory
WHERE Movies.MovieID=Inventory.MovieID
Group by MovieName HAVING count(TapeID)>1;
```
(7) 检索出租给顾客的持续时间最长的影片的片名。
```sql
SELECT MovieName
FROM Movies,Inventory,Rentals
WHERE Inventory.TapeID=Rentals.TapeID and
      Movies.movieID=Inventory.TapeID and
      Duration=(
        SELECT MAX(Duration)
        FROM Rentals
      );
```
(8) 检索不在库存中的所有影片片名。
```sql
SELECT MovieName
FROM Movies
WHERE MovieID <>ALL(
  SELECT DISTINCT MovieID
  FROM Inventory
)
```
(9) 检索名为"Hacksaw Ridge "的影片给出的售价最低的供货商号及该供货商名。
```sql
SELECT SupplierID,SupplierName
FROM Movies,Suppliers,MovieSupplier
WHERE Movies.MovieName='Hacksaw Ridge' and
      Movies.MovieID=MovieSupplier.MovieID and
      Supplier.SupplierID=MovieSupplier.SupplierID and
      Price=(
        SELECT MIN(Price)
        FROM MovieSupplier
      );
```
(10) 检索租用了名为"Beauty and the Beast"的影片或者租用了由名为"VWS Video"的供货商提供的影片的那些顾客的姓名。 
```sql
SELECT LastName,FirstName
FROM Customers,Rentals
WHERE TapeID IN (
        SELECT TapeID
        FROM Movies,Inventory
        WHERE MovieName='Beauty and the Beast' and
              Movies.MovieID=Inventory.MovieID
      ) OR
      TapeID IN (
        SELECT TapeID
        FROM Suppliers,Orders,Inventory
        WHERE SupplierName='VWS Video' and
              Suppliers.SupplierID=MovieSupplier.SupplierID and
              Inventory.MovieID=MovieSupplier.MovieID
      )
```