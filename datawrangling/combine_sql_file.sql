SELECT *
FROM (
 (
		SELECT	60 as Test,
		'Hello' as TextSample,
		50 as emptyCol
  ) UNION (
		select 	100 ,
		'DSTI',
		60.3
  )
) AS temp
WHERE temp.TextSample = 'DSTI'

-- SELECT 60 as Test, 'Hello' as TextSample, NULL as emptyCol;

/*
--2.
*/
SELECT
	c2.CustomerId,  c2.CustomerName, c1.CustomerName as BillToCustomerName
FROM 
	Sales.Customers as c1,
	Sales.Customers as c2
 WHERE
	c2.BillToCustomerID = c1.CustomerID
	AND c2.CustomerName <> c1.CustomerName


-- All the products which are greater or equal to the price of 'DBA joke mug - daaaaaa-ta (White)'
DECLARE @productName nvarchar(100);
SET @productName = 'DBA joke mug - daaaaaa-ta (White)';


Select s2.StockItemID, s2.StockItemName, s2.UnitPrice 
  FROM [Warehouse].[StockItems] as s1,
  [Warehouse].[StockItems] as s2

  where
	s1.StockItemName = @productName
	AND s2.UnitPrice >= s1.UnitPrice
	AND s2.StockItemID <> s1.StockItemID

order by s2.UnitPrice

/*
3.
	Extract all customer id, names and their postal city NAME
*/

SELECT
		Cu.CustomerID,
		Cu.CustomerName,
		Ci.CityName
FROM
		Sales.Customers AS Cu,
		[Application].Cities AS Ci
WHERE
		Cu.PostalCityID = Ci.CityID ;


SELECT
		Cu.CustomerID,
		Cu.CustomerName,
		Ci.CityName
FROM
		Sales.Customers as Cu INNER JOIN
		[Application].Cities AS Ci 
		ON (Cu.PostalCityID = Ci.CityID) ;

/*
4.
-- Are the records in invoice, based on the duet
-- CustomerId + BillToCustomerId consistent with the same duet
-- in the table Customer
-- this needs done as the table Invoice is in breach of normalisation
*/

SELECT i.*
FROM
	Sales.Invoices as i,
	Sales.Customers as c
WHERE
	i.CustomerID = c.CustomerID
	and i.BillToCustomerID <> c.BillToCustomerID;

/*
PlayingWithDates.sql
-- Standard SQL
*/

select *
from Sales.Customers
where YEAR(AccountOpenedDate) >= 2014
		and month(AccountOpenedDate) >= 1
		and day(AccountOpenedDate) >= 1;

-- MSSQL Specific
declare @UKFormat int;
set @UKFormat = 103;

select *
from Sales.Customers
where AccountOpenedDate 
	between CONVERT(date,'01/01/2014',@UKFormat)
	and convert(date, '30/06/2014',@UKFormat);

/*
CountCustomersAndCities-NoJoin.sql
-- Count the number of cust per PostalCityId (no join)
*/

SELECT
		COUNT(*) as NbCustomers,	--counts the number of rows
					-- return an integer, scalar, dimension = 1
		Ci.CityName, -- dimension = n rows
		Cu.PostalCityID
FROM
	Sales.Customers as Cu,
	[Application].Cities as Ci
WHERE
	Cu.PostalCityID = ci.CityID
GROUP BY Ci.CityName, Cu.PostalCityID	--instructs the engine to "packet" of
						-- PostalCityId
						-- "distinct" all PostalCityId
						-- for COUNT to have a distinct value
ORDER BY NbCustomers DESC, Ci.CityName ASC

/*
CountCustomersAndCities-Join.sql
-- Count the number of custumers per CityName (join)
*/

SELECT
		COUNT(*) as NbCustomers,	--counts the number of rows
					-- return an integer, scalar, dimension = 1
		Ci.CityName, -- dimension = n rows
		Cu.PostalCityID
FROM
	Sales.Customers as Cu JOIN
	[Application].Cities as Ci ON (Cu.PostalCityID = ci.CityID)

GROUP BY Ci.CityName, Cu.PostalCityID	--instructs the engine to "packet" of
						-- PostalCityId
						-- "distinct" all PostalCityId
						-- for COUNT to have a distinct value
ORDER BY NbCustomers DESC, Ci.CityName ASC

/*
CreatingSomeDataInconsistenciesWithUpdate.sql
/****** Script for SelectTopNRows command from SSMS  ******/
*/

SELECT *
  FROM [WideWorldImporters-Seb].[Sales].[Customers]
  where CustomerID = 832


-- Inconsistency in Customers table
  update sales.Customers
  set BillToCustomerID = 1
  where CustomerID = 832
  -- Run the consistency query to observe the results

  -- Inconsistency in Invoice table
  update sales.Invoices
  set BillToCustomerID = 1
  where InvoiceID = 1
 -- Run the consistency query to observe the results

-- Back to normal in Customers table
  update sales.Customers
  set BillToCustomerID = 832
  where CustomerID = 832
 -- Run the consistency query to observe the results

  -- Back to normal in Invoice table
  update sales.Invoices
  set BillToCustomerID = 832
  where InvoiceID = 1
 -- Run the consistency query to observe the results

/*
ExistanceQuery.sql
-- Q3: Is there any Cities in the Application.Cities tables
-- unreferenced by Sales.Customers

-- (non) EXISTANCE QUERY
*/

SELECT COUNT(DISTINCT PostalCityId) -- = 655
from Sales.Customers

SELECT COUNT(CityId) -- = 37940
from [Application].Cities

-- Good case
/* Possible but overkill
select Ci.CityID, Cu.PostalCityID,
	Ci.CityName, Cu.CustomerID, Cu.CustomerName

from
	[Application].Cities as ci LEFT OUTER JOIN Sales.Customers as Cu
	on (Ci.CityID = Cu.PostalCityID)
*/
SELECT
		count(Ci.CityID)
FROM
	[Application].Cities as Ci
WHERE NOT EXISTS
	(
		SELECT *
		FROM Sales.Customers as Cu
		WHERE
			Cu.PostalCityID = Ci.CityID
	)


/*
5. PivotOnInvoice-UNION.sql

-- Compute the turnover by customer on the whole period of time
-- CustomerId, CustomerName, SalesTotal
*/

SELECT
		C.CustomerID, C.CustomerName
		, SUM(il.Quantity * il.UnitPrice) as SalesTotal
FROM
	Sales.Invoices as i,
	Sales.InvoiceLines as il,
	Sales.Customers as c
WHERE
	i.InvoiceID = il.InvoiceID
	and i.CustomerID = c.CustomerID

GROUP BY C.CustomerId, C.CustomerName
ORDER BY SalesTotal DESC, C.CustomerName;

-- Compute the turnover by customer for 2014, 2015 and 2016 in columns
-- CustomerId, CustomerName, Sales2014, Sales2015, Sales2016
SELECT	temp.CustomerID, temp.CustomerName,
		SUM(temp.Sales2014) as Sales2014,
		SUM(temp.Sales2015) as Sales2015,
		SUM(temp.Sales2016) as Sales2016,
		SUM(Sales2014+Sales2015+Sales2016) as TotalSales,
		AVG(Sales2014+Sales2015+Sales2016) as AvgSales
FROM
(
	(
		SELECT
				C.CustomerID, C.CustomerName,
				SUM(il.Quantity * il.UnitPrice) as Sales2014,
				0 as Sales2015,
				0 as Sales2016
		FROM
			Sales.Invoices as i,
			Sales.InvoiceLines as il,
			Sales.Customers as c
		WHERE
			i.InvoiceID = il.InvoiceID
			and i.CustomerID = c.CustomerID
			and year(i.Invoicedate) = 2014

		GROUP BY C.CustomerId, C.CustomerName
		--ORDER BY Sales2014 DESC, C.CustomerName
	)
	UNION
	(
		SELECT
			C.CustomerID, C.CustomerName,
			0 as Sales2014,
			SUM(il.Quantity * il.UnitPrice) as Sales2015,
			0 as Sales2016
		FROM
			Sales.Invoices as i,
			Sales.InvoiceLines as il,
			Sales.Customers as c
		WHERE
			i.InvoiceID = il.InvoiceID
			and i.CustomerID = c.CustomerID
			and year(i.Invoicedate) = 2015

		GROUP BY C.CustomerId, C.CustomerName
		--ORDER BY Sales2014 DESC, C.CustomerName
	)
	UNION
	(
		SELECT
			C.CustomerID, C.CustomerName,
			0 as Sales2014,
			0 as Sales2015,
			SUM(il.Quantity * il.UnitPrice) as Sales2016
		FROM
			Sales.Invoices as i,
			Sales.InvoiceLines as il,
			Sales.Customers as c
		WHERE
			i.InvoiceID = il.InvoiceID
			and i.CustomerID = c.CustomerID
			and year(i.Invoicedate) = 2016

		GROUP BY C.CustomerId, C.CustomerName
		--ORDER BY Sales2014 DESC, C.CustomerName
	)
)as temp
GROUP BY temp.CustomerID, temp.CustomerName
ORDER BY AvgSales desc, temp.CustomerName


/*
6: PivotOnInvoice-SubQueries-In-Select.sql
-- Compute the turnover by customer on the whole period of time
-- CustomerId, CustomerName, SalesTotal
*/
SELECT
		Cu.CustomerID,
		Cu.CustomerName,
		(
			SELECT	SUM(il.Quantity * il.UnitPrice)
			FROM	Sales.Invoices as i,
					Sales.InvoiceLines as il
			WHERE
					cu.CustomerID = i.CustomerID
					AND il.InvoiceID = i.InvoiceID
					AND YEAR(i.InvoiceDate) = 2014
		) as Sales2014,
		(
			SELECT	SUM(il.Quantity * il.UnitPrice)
			FROM	Sales.Invoices as i,
					Sales.InvoiceLines as il
			WHERE
					cu.CustomerID = i.CustomerID
					AND il.InvoiceID = i.InvoiceID
					AND YEAR(i.InvoiceDate) = 2015
		) as Sales2015,
		(
			SELECT	SUM(il.Quantity * il.UnitPrice)
			FROM	Sales.Invoices as i,
					Sales.InvoiceLines as il
			WHERE
					cu.CustomerID = i.CustomerID
					AND il.InvoiceID = i.InvoiceID
					AND YEAR(i.InvoiceDate) = 2016
		) as Sales2016,
		(
			SELECT	SUM(il.Quantity * il.UnitPrice)
			FROM	Sales.Invoices as i,
					Sales.InvoiceLines as il
			WHERE
					cu.CustomerID = i.CustomerID
					AND il.InvoiceID = i.InvoiceID
					AND YEAR(i.InvoiceDate) between 2014 and 2016
		) as TotalSales,
		(
			SELECT	SUM(il.Quantity * il.UnitPrice) as totals
			FROM	Sales.Invoices as i,
					Sales.InvoiceLines as il
			WHERE
					cu.CustomerID = i.CustomerID
					AND il.InvoiceID = i.InvoiceID
					AND YEAR(i.InvoiceDate) between 2014 and 2016
		)/3 as SimplerAvg,
		(
			SELECT AVG(t.sumSales)
			FROM
				(
					SELECT SUM(il.Quantity * il.UnitPrice) as sumSales
					FROM	Sales.Invoices as i,
							Sales.InvoiceLines as il
					WHERE
							cu.CustomerID = i.CustomerID
							AND il.InvoiceID = i.InvoiceID
							AND YEAR(i.InvoiceDate) between 2014 and 2016
					GROUP BY YEAR(i.InvoiceDate)
				) as t
		) as AvgSales
FROM
	Sales.Customers as Cu

/*
offendin_customers.sql
-- ORDERS NOT CONVERTED INTO INVICES
*/

SELECT COUNT(*)
FROM
	Sales.Orders as o;

SELECT	o.CustomerID,c.CustomerName,
		p.FullName,
		p.EmailAddress,
		p.PhoneNumber,	
		sum(ol.Quantity*ol.UnitPrice) as TotalLoss
		-- name, phone and email from main contact of customer
FROM
	Sales.Orders as o,
	Sales.OrderLines as ol,
	Sales.Customers as c,
	[Application].People as p
WHERE NOT EXISTS
(	
	SELECT *
	FROM Sales.Invoices as i
	WHERE
		o.OrderID = i.OrderID
) AND ol.OrderID = o.OrderID
AND c.CustomerID = o.CustomerID
AND c.PrimaryContactPersonID = p.PersonID

group by o.CustomerID, c.CustomerName, p.FullName, p.EmailAddress,p.PhoneNumber
order by TotalLoss desc;

/*
Division_SQL.sql
*/

select *
from dbo.Customer as c
where not exists
(
	select *
	from dbo.Product as p
	where 
	p.ProductName like 'Laptop%' and
	not exists
	(
		select *
		from dbo.Purchase as pu
		where pu.CustomerId = c.CustomerId
		and pu.ProductId = p.ProductId
		-- extra restriction: more than 1 product purchased
		and pu.Qty > 1
	)
)

/****** Object:  Table [dbo].[Student]    Script Date: 24/11/2018 15:57:24 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[Student](
	[email] [nvarchar](50) NOT NULL,
	[FirstName] [nvarchar](50) NULL,
	[LastName] [nvarchar](50) NULL,
	[Programme] [nvarchar](50) NULL,
	[CohortYear] [int] NULL,
	[ApplicationStatus] [int] NULL,
	[StudentNumber] [nvarchar](8) NULL,
 CONSTRAINT [PK_Student] PRIMARY KEY CLUSTERED 
(
	[email] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
GO

/*
transactionExample.sql
*/
insert into Student( 
					FirstName, LastName, Programme,
					CohortYear,ApplicationStatus)
values('sebastien@dsti.co', 'S�bastien', 'Corniglion',
		'Applied MSc Data Engineering',
		2018, 1); --application status 1 = applicant, 2 admitted

insert into Student(email, 
					FirstName, LastName,
					CohortYear,ApplicationStatus)
values('vincent@dsti.co', 'Vincent', 'Pressicaud',
		2018, NULL); --application status 1 = applicant, 2 admitted

SET TRANSACTION ISOLATION LEVEL SNAPSHOT;
BEGIN TRANSACTION;

select * from Student;

insert into Student(email, 
					FirstName, LastName, Programme,
					CohortYear,ApplicationStatus)
values('leo@dsti.co', 'L�o', 'Souquet',
		'Applied MSc Data Engineering',
		2018, NULL); --application status 1 = applicant, 2 admitted

update Student set Programme = 'Applied MSc Data Science'
where email = 'leo@dsti.co'
select * from Student;
commit;
rollback;

outsideTransactionScript.sql
select * from Student;

insert into Student(email, 
					FirstName, LastName, Programme,
					CohortYear,ApplicationStatus)
values('leo@dsti.co', 'L�o', 'Souquet',
		'Applied MSc Data Engineering',
		2018, NULL);

update Student set Programme = 'Applied MSc Data Science'
where email = 'sebastien@dsti.co'

Script_Creation_Student_Table.sql
USE [Sqlplayground_a18]
GO

/*
firstTriggerStudentNumber.sql
-- ================================================
-- Template generated from Template Explorer using:
-- Create Trigger (New Menu).SQL
--
-- Use the Specify Values for Template Parameters 
-- command (Ctrl-Shift-M) to fill in the parameter 
-- values below.
--
-- See additional Create Trigger templates for more
-- examples of different Trigger statements.
--
-- This block of comments will not be included in
-- the definition of the function.
-- ================================================
*/

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	This trigger fires on update when the column ApplicationStatus
--				becomes = 2
-- =============================================
ALTER TRIGGER dbo.trg_AllocateStudentNumber 
   ON  dbo.Student 
   AFTER UPDATE --the trigger fires on ALL updates for this table, 
   --regardless of the column
AS 
BEGIN
	DECLARE @currentApplicationStatus int;
	DECLARE @currentCohortYear int;
	DECLARE @lastSequenceNumber int;
	DECLARE @strSequenceNumber nvarchar(3);
	DECLARE @currentEmail nvarchar(50);

	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;


	IF UPDATE(ApplicationStatus) -- Check which column is updated
	BEGIN

		SET @currentApplicationStatus = (SELECT ApplicationStatus FROM inserted);

		IF @currentApplicationStatus = 2
		BEGIN

			set @currentCohortYear = (Select CohortYear from inserted);
			--now we need the last sequence number

			--student numbers look like 2019003
			set @lastSequenceNumber = (select MAX(convert(int, RIGHT(COALESCE(StudentNumber,0),3))) 
										from dbo.Student);
			set @currentEmail = (select email from inserted);

			SET @lastSequenceNumber = @lastSequenceNumber + 1
			SET @strSequenceNumber = FORMAT(@lastSequenceNumber, '00#');

			BEGIN TRANSACTION;

			BEGIN TRY
				UPDATE dbo.Student
				SET StudentNumber = convert(nvarchar,@currentCohortYear) + @strSequenceNumber
				WHERE email = @currentEmail;

				COMMIT;
			END TRY
			BEGIN CATCH

				ROLLBACK;
			END CATCH;

		END;


	END;


END;
GO

/*
transactionExample.sql
*/

insert into Student( 
					FirstName, LastName, Programme,
					CohortYear,ApplicationStatus)
values('sebastien@dsti.co', 'Sébastien', 'Corniglion',
		'Applied MSc Data Engineering',
		2018, 1); --application status 1 = applicant, 2 admitted

insert into Student(email, 
					FirstName, LastName,
					CohortYear,ApplicationStatus)
values('vincent@dsti.co', 'Vincent', 'Pressicaud',
		2018, NULL); --application status 1 = applicant, 2 admitted

SET TRANSACTION ISOLATION LEVEL SNAPSHOT;
BEGIN TRANSACTION;

select * from Student;

insert into Student(email, 
					FirstName, LastName, Programme,
					CohortYear,ApplicationStatus)
values('leo@dsti.co', 'Léo', 'Souquet',
		'Applied MSc Data Engineering',
		2018, NULL); --application status 1 = applicant, 2 admitted

update Student set Programme = 'Applied MSc Data Science'
where email = 'leo@dsti.co'
select * from Student;
commit;
rollback;

/*
outsideTransactionScript.sql
*/
select * from Student;

insert into Student(email, 
					FirstName, LastName, Programme,
					CohortYear,ApplicationStatus)
values('leo@dsti.co', 'Léo', 'Souquet',
		'Applied MSc Data Engineering',
		2018, NULL);

update Student set Programme = 'Applied MSc Data Science'
where email = 'sebastien@dsti.co'

/*
FullTriggerMultirows.sql
*/

USE [Sqlplayground_a18]
GO
/****** Object:  Trigger [dbo].[trg_AllocateStudentNumber_V2]    Script Date: 27/11/2018 11:22:20 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	This trigger fires on update when the column ApplicationStatus
--				becomes = 2
-- =============================================
ALTER TRIGGER [dbo].[trg_AllocateStudentNumber_V2] 
   ON  [dbo].[Student] 
   AFTER UPDATE --the trigger fires on ALL updates for this table, 
   --regardless of the column
AS 
BEGIN
	DECLARE @currentApplicationStatus int;
	DECLARE @currentCohortYear int;
	DECLARE @lastSequenceNumber int;
	DECLARE @strSequenceNumber nvarchar(3);
	DECLARE @currentEmail nvarchar(50);
	DECLARE @currentStudentNumber nvarchar(8);

	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	IF @@NESTLEVEL < 2 -- don't run at all if you are not the initial trigger call
	BEGIN
		IF UPDATE(ApplicationStatus)  -- Check which column is updated
		BEGIN
			
			-- BE CAREFUL, NO @ in front of a cursor name
			DECLARE insertedCursor CURSOR FOR 
					SELECT ApplicationStatus, CohortYear, email, StudentNumber FROM inserted;
			OPEN insertedCursor;

			-- fetch the new row in the cursor into local variables
			-- be careful on the order of the local variables
			-- they must match the cursor header declaration
			FETCH NEXT FROM insertedCursor INTO @currentApplicationStatus,
												@currentCohortYear,
												@currentEmail,
												@currentStudentNumber;

			--@@FETCH_STATUS = 0 means there is a row in the cursor
			WHILE @@FETCH_STATUS = 0
			BEGIN

				-- no need, given by iterating over the cursor
				--SET @currentApplicationStatus = (SELECT ApplicationStatus FROM inserted);
		
				IF @currentApplicationStatus = 2 and @currentStudentNumber IS NULL
				BEGIN
					-- no need, given by iterating over the cursor
					--set @currentCohortYear = (Select CohortYear from inserted);
					
					--now we need the last sequence number

					--student numbers look like 2019003
					set @lastSequenceNumber = (select MAX(convert(int, RIGHT(COALESCE(StudentNumber,0),3))) 
												from dbo.Student) + 1;
					
					-- no need, given by iterating over the cursor
					--set @currentEmail = (select email from inserted);

					--SET @lastSequenceNumber = @lastSequenceNumber + 1
					SET @strSequenceNumber = FORMAT(@lastSequenceNumber, '00#');

					BEGIN TRANSACTION;

					BEGIN TRY
						UPDATE dbo.Student
						SET StudentNumber = convert(nvarchar,@currentCohortYear) + @strSequenceNumber
						WHERE email = @currentEmail;

						COMMIT;
					END TRY
					BEGIN CATCH
						-- "Returns" the detailled reasons for error
								-- Look at this pasge for the meaning of all the error description
								-- https://docs.microsoft.com/en-us/sql/t-sql/language-elements/try-catch-transact-sql
								SELECT  ERROR_NUMBER() AS ErrorNumber  
										,ERROR_SEVERITY() AS ErrorSeverity  
										,ERROR_STATE() AS ErrorState  
										,ERROR_PROCEDURE() AS ErrorProcedure  
										,ERROR_LINE() AS ErrorLine  
										,ERROR_MESSAGE() AS ErrorMessage;

						ROLLBACK;
					END CATCH;
				END;

				FETCH NEXT FROM insertedCursor INTO @currentApplicationStatus,
												@currentCohortYear,
												@currentEmail,
												@currentStudentNumber;
				
			END; -- WHILE

			--IMPORTANT - DO NOT FORGET
			-- Close the iterator
			CLOSE insertedCursor;
			-- Release memory (and potential locks, if any)
			DEALLOCATE insertedCursor;
		END;
	END;
END;

/*
DynamicSQL_BasicExample.sql
*/
-- sp_executesql is a system stored procedure
-- requires at least an nvarchar containing a valid statement
exec sp_executesql N'SELECT * FROM Sales.Customers';

-- Dynamically parameterise the query
DECLARE @ParmDefinition nvarchar(200);
SET @ParmDefinition = '@custid int'

-- make sure that parameters are always named, may work after a first execution
-- but unreliable
exec sp_executesql N'SELECT * FROM Sales.Customers where CustomerId = @custid;', 
					@ParmDefinition, 
					@custid=60;

DECLARE @ParmDefinition nvarchar(200);
SET @ParmDefinition = '@startDate int, @endDate int'
DECLARE @startDate int;
DECLARE @dynSQL as nvarchar(4000);
SET @dynSQL = 'SELECT sum(il.Quantity * il.UnitPrice)
				FROM Sales.Invoices as i, Sales.InvoiceLines as il 
				WHERE il.InvoiceID = i .InvoiceID
				AND year(i.InvoiceDate) BETWEEN @startDate AND @endDate';
exec sp_executesql @dynSQL, @ParmDefinition, 2014,
							2016

DECLARE @dynSQL2 as nvarchar(4000);
SET @dynSQL2 = 'SELECT sum(il.Quantity * il.UnitPrice)
				FROM Sales.Invoices as i, Sales.InvoiceLines as il 
				WHERE il.InvoiceID = i .InvoiceID
				AND year(i.InvoiceDate) = <YEAR_VALUE>';
set @dynSQL2 = REPLACE(@dynSQL2, '<YEAR_VALUE>', '2014');
exec sp_executesql @dynSQL2;

/*
Pivoting_RowBased_SurveyData_Static_Query.sql
*/
(
SELECT
		UserId,
		1 as SurveyId,
		COALESCE(	(
						SELECT	Answer_Value
						FROM	Answer as a
						WHERE	a.UserId = u.UserId
								AND a.SurveyId = 1
								AND a.QuestionId = 1
					), -1) AS A1,
		COALESCE(	(
						SELECT	Answer_Value
						FROM	Answer as a
						WHERE	a.UserId = u.UserId
						AND		a.SurveyId = 1
						AND		a.QuestionId = 2
					), -1) AS A2,
		NULL as A3
FROM
		[User] as u
WHERE EXISTS
(
	SELECT *
	From Answer as a
	where
		u.UserId = a.UserId
		AND a.SurveyId = 1
)
)
UNION
(
SELECT
		UserId,
		2 as SurveyId,
		NULL as A1,
		COALESCE(	(
						SELECT	Answer_Value
						FROM	Answer as a
						WHERE	a.UserId = u.UserId
								AND a.SurveyId = 2
								AND a.QuestionId = 2
					), -1) AS A2,
		COALESCE(	(
						SELECT	Answer_Value
						FROM	Answer as a
						WHERE	a.UserId = u.UserId
						AND		a.SurveyId = 2
						AND		a.QuestionId = 3
					), -1) AS A3
FROM
		[User] as u
WHERE EXISTS
(
	SELECT *
	From Answer as a
	where
		u.UserId = a.UserId
		AND a.SurveyId = 2
)
)
ORDER BY SurveyId, UserId

/*
FullDynamicQuery-SurveyData.sql
*/
USE [Survey_Sample_A18]
GO
/****** Object:  StoredProcedure [dbo].[getAllSurveyAnswerData]    Script Date: 29/11/2018 10:02:35 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[getAllSurveyAnswerData_FINAL] 
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;
	
	DECLARE @strTemplateUnionBase nvarchar(max);
	DECLARE @strTemplateNULLSelectBlock nvarchar(max);
	DECLARE @strTemplateCoalesceSelectBlock nvarchar(max);

	DECLARE @currentSurveyId int;
	DECLARE @currentQuestionId int;

	DECLARE @isQuestionInSurvey int;

	DECLARE @strCurrentUnionBloc nvarchar(max);
	DECLARE @strCurrentSelectBlock nvarchar(max);

	DECLARE @strFinalQuery nvarchar(max);

	DECLARE @MaxSurveyId int;
	DECLARE @loop int;

	SET @strFinalQuery = '';
	
	SET @MaxSurveyId = (SELECT MAX(SurveyId) FROM Survey);
	SET @loop = 1;


	SET @strTemplateNULLSelectBlock = ' NULL as A<QUESTION_ID> ';
	SET @strTemplateCoalesceSelectBlock = '
								COALESCE(	(
								SELECT	Answer_Value
								FROM	Answer as a
								WHERE	a.UserId = u.UserId
										AND a.SurveyId = <SURVEY_ID>
										AND a.QuestionId = <QUESTION_ID>
							), -1) AS A<QUESTION_ID> ';

	SET @strTemplateUnionBase ='
	(
		SELECT
				UserId,
				<SURVEY_ID> as SurveyId,
				<VARIADIC_SELECT_BLOCK>
		FROM
				[User] as u
		WHERE EXISTS
		(
			SELECT *
			FROM Answer as a
			WHERE
				u.UserId = a.UserId
				AND a.SurveyId = <SURVEY_ID>
		)
	)';

	DECLARE surveyCursor CURSOR FOR
					SELECT	SurveyId
					FROM	Survey
					ORDER BY SurveyId;
	OPEN surveyCursor;

	FETCH NEXT FROM surveyCursor INTO @currentSurveyId;

	WHILE @@FETCH_STATUS = 0
	BEGIN
		
		SET @strCurrentUnionBloc = REPLACE(@strTemplateUnionBase, 
										'<SURVEY_ID>',
										convert(nvarchar(max), @currentSurveyId)
										);

		SET @strFinalQuery = @strFinalQuery + @strCurrentUnionBloc
		

		SET @strCurrentSelectBlock = '';
		DECLARE questionCursor CURSOR FOR
							SELECT	q.QuestionId
							FROM Question as q, SurveyStructure as s
							--WHERE  EXISTS
							--(
							--	SELECT *
							--	FROM SurveyStructure as s
							--	WHERE s.QuestionId = q.QuestionId
							--)
							WHERE
								s.QuestionId = q.QuestionId
							ORDER BY q.QuestionId;
		OPEN questionCursor;

		FETCH NEXT FROM questionCursor INTO @currentQuestionId;
		WHILE @@FETCH_STATUS = 0
		BEGIN
			
			--check whether the currentQuestionId is part of the the currentSurveyId
			SET @isQuestionInSurvey = ( SELECT	COUNT(QuestionId)
			FROM	SurveyStructure
			WHERE	QuestionId = @currentQuestionId
					AND SurveyId = @currentSurveyId )

			IF @isQuestionInSurvey > 0
			BEGIN
				-- COALESCE TEMPLATE
				SET @strCurrentSelectBlock = @strCurrentSelectBlock + REPLACE(@strTemplateCoalesceSelectBlock,
																		'<QUESTION_ID>',
																		convert(nvarchar(max), @currentQuestionId)
																	);
			END;
			ELSE
			BEGIN
				-- NULL TEMPLATE
				SET @strCurrentSelectBlock = @strCurrentSelectBlock + REPLACE(@strTemplateNULLSelectBlock,
																		'<QUESTION_ID>',
																		convert(nvarchar(max), @currentQuestionId)
																	);
			
			END;

			SET @strCurrentSelectBlock = REPLACE(@strCurrentSelectBlock, '<SURVEY_ID>', @currentSurveyId);

			FETCH NEXT FROM questionCursor INTO @currentQuestionId;
			IF @@FETCH_STATUS = 0
			BEGIN
				SET @strCurrentSelectBlock = @strCurrentSelectBlock +  ' , '
			END;
			
		END;
		CLOSE questionCursor;
		DEALLOCATE questionCursor;

		--SELECT @strCurrentSelectBlock;

		
		FETCH NEXT FROM surveyCursor INTO @currentSurveyId;
		IF @@FETCH_STATUS = 0
		BEGIN
			SET @strFinalQuery = @strFinalQuery + ' UNION '
		END;

		SET @strFinalQuery = REPLACE(@strFinalQuery, '<VARIADIC_SELECT_BLOCK>', @strCurrentSelectBlock);

	END;

	

	close surveyCursor;
	deallocate surveyCursor;

	--SELECT @strFinalQuery;
	SET @strFinalQuery = @strFinalQuery + ' ORDER BY SurveyId, UserId';
	EXEC sp_executesql @strFinalQuery;
    
END

/*
ViewQuery-Schemabinding.sql
*/
CREATE OR ALTER VIEW v_SurveyTakers WITH SCHEMABINDING AS
	SELECT s.SurveyId, s.SurveyDescription, u.[User_Name], u.User_Email,
	u.UserId
	FROM 
		dbo.Survey as s,
		dbo.[User] as u
	WHERE
		s.Survey_UserAdminId = u.UserId
