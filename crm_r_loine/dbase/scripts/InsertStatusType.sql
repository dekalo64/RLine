USE [VL]
GO
/****** Объект:  StoredProcedure [dbo].[InsertStatusType]    Дата сценария: 04/26/2013 11:10:03 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

--
-- Definition for stored procedure InsertStatusType : 
--

--
-- Definition for stored procedure InsertStatusType : 
--

CREATE PROCEDURE [dbo].[InsertStatusType]
(@st_name           udd_name,
 @st_actual         udd_logical)

AS

/*      Set stamp variables     */
DECLARE @st_mdate      udd_date,
        @st_muser      udd_user

SELECT
   @st_muser = USER_ID(),
   @st_mdate = GETDATE()

BEGIN TRANSACTION

/*      Recall deleted record   */
UPDATE StatusType
SET StatusType.st_name     = @st_name,
	StatusType.st_muser    = @st_muser,
	StatusType.st_mdate    = @st_mdate,
	StatusType.st_deleted  = 0,
    StatusType.st_actual   = @st_actual
WHERE StatusType.st_code = (SELECT MIN(StatusType.st_code)
                       FROM  StatusType
                       WHERE StatusType.st_deleted = 1)

/*      Insert new record       */
IF (@@rowcount < 1)
BEGIN
   INSERT INTO StatusType
      SELECT
         st_code        = StatusType.st_code + 1,
         st_name        = @st_name,
		 st_deleted     = 0,
		 st_muser       = @st_muser,
         st_mdate       = @st_mdate,
         st_actual      = @st_actual   
      FROM  StatusType
      WHERE StatusType.st_code = (SELECT MAX(StatusType.st_code)
                             FROM StatusType)

/*      Insert first record     */
   IF (@@rowcount < 1)
   BEGIN
      INSERT INTO StatusType (
		 st_code,
		 st_name,
		 st_deleted,
	     st_muser,
         st_mdate,
         st_actual)
      VALUES (
         1,
		 @st_name,
		 0,
	     @st_muser,
         @st_mdate,
         @st_actual)
   END
END

/*      Return result and check         */
/*      existence of parent record      */
SELECT
   st_code    = StatusType.st_code,
   st_name    = StatusType.st_name
FROM  StatusType
WHERE StatusType.st_mdate      = @st_mdate
  AND StatusType.st_muser      = @st_muser
  AND StatusType.st_deleted    = 0

/*      Finish transaction              */
IF (@@rowcount > 0) COMMIT   TRANSACTION
ELSE                ROLLBACK TRANSACTION

