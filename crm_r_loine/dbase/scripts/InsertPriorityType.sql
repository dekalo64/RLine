SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER OFF
GO


--
-- Definition for stored procedure InsertPriorityType : 
--

--
-- Definition for stored procedure InsertPriorityType : 
--

CREATE PROCEDURE [dbo].[InsertPriorityType]
(@pt_name           udd_name,
 @pt_actual         udd_logical,
 @pt_level          udd_code)

AS

/*      Set stamp variables     */
DECLARE @pt_mdate      udd_date,
        @pt_muser      udd_user

SELECT
   @pt_muser = USER_ID(),
   @pt_mdate = GETDATE()

BEGIN TRANSACTION

/*      Recall deleted record   */
UPDATE PriorityType
SET PriorityType.pt_name     = @pt_name,
	PriorityType.pt_muser    = @pt_muser,
	PriorityType.pt_mdate    = @pt_mdate,
	PriorityType.pt_deleted  = 0,
    PriorityType.pt_actual   = @pt_actual,
    PriorityType.pt_level    = @pt_level
WHERE PriorityType.pt_code = (SELECT MIN(PriorityType.pt_code)
                       FROM  PriorityType
                       WHERE PriorityType.pt_deleted = 1)

/*      Insert new record       */
IF (@@rowcount < 1)
BEGIN
   INSERT INTO PriorityType
      SELECT
         pt_code        = PriorityType.pt_code + 1,
         pt_name        = @pt_name,
		 pt_deleted     = 0,
		 pt_muser       = @pt_muser,
         pt_mdate       = @pt_mdate,
         pt_actual      = @pt_actual,
         pt_level       = @pt_level   
      FROM  PriorityType
      WHERE PriorityType.pt_code = (SELECT MAX(PriorityType.pt_code)
                             FROM PriorityType)

/*      Insert first record     */
   IF (@@rowcount < 1)
   BEGIN
      INSERT INTO PriorityType (
		 pt_code,
		 pt_name,
		 pt_deleted,
	     pt_muser,
         pt_mdate,
         pt_actual,
         pt_level)
      VALUES (
         1,
		 @pt_name,
		 0,
	     @pt_muser,
         @pt_mdate,
         @pt_actual,
         @pt_level)
   END
END

/*      Return result and check         */
/*      existence of parent record      */
SELECT
   pt_code    = PriorityType.pt_code,
   pt_name    = PriorityType.pt_name,
   pt_muser   = PriorityType.pt_muser,
   pt_mdate   = PriorityType.pt_mdate,
   pt_actual  = PriorityType.pt_actual,
   pt_level   = PriorityType.pt_level   

FROM  PriorityType
WHERE PriorityType.pt_mdate      = @pt_mdate
  AND PriorityType.pt_muser      = @pt_muser
  AND PriorityType.pt_deleted    = 0

/*      Finish transaction              */
IF (@@rowcount > 0) COMMIT   TRANSACTION
ELSE                ROLLBACK TRANSACTION

