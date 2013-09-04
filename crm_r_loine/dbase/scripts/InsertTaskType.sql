USE [VL]
GO
/****** Объект:  StoredProcedure [dbo].[InsertTaskType]    Дата сценария: 04/25/2013 22:22:46 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

--
-- Definition for stored procedure InsertTaskType : 
--

--
-- Definition for stored procedure InsertTaskType : 
--

CREATE PROCEDURE [dbo].[InsertTaskType]
(@tt_name           udd_name,
 @tt_actual         udd_logical)

AS

/*      Set stamp variables     */
DECLARE @tt_mdate      udd_date,
        @tt_muser      udd_user

SELECT
   @tt_muser = USER_ID(),
   @tt_mdate = GETDATE()

BEGIN TRANSACTION

/*      Recall deleted record   */
UPDATE TaskType
SET TaskType.tt_name     = @tt_name,
	TaskType.tt_muser    = @tt_muser,
	TaskType.tt_mdate    = @tt_mdate,
	TaskType.tt_deleted  = 0,
    TaskType.tt_actual   = @tt_actual
WHERE TaskType.tt_code = (SELECT MIN(TaskType.tt_code)
                       FROM  TaskType
                       WHERE TaskType.tt_deleted = 1)

/*      Insert new record       */
IF (@@rowcount < 1)
BEGIN
   INSERT INTO TaskType
      SELECT
         tt_code        = TaskType.tt_code + 1,
         tt_name        = @tt_name,
		 tt_deleted     = 0,
		 tt_muser       = @tt_muser,
         tt_mdate       = @tt_mdate,
         tt_actual      = @tt_actual   
      FROM  TaskType
      WHERE TaskType.tt_code = (SELECT MAX(TaskType.tt_code)
                             FROM TaskType)

/*      Insert first record     */
   IF (@@rowcount < 1)
   BEGIN
      INSERT INTO TaskType (
		 tt_code,
		 tt_name,
		 tt_deleted,
	     tt_muser,
         tt_mdate,
         tt_actual)
      VALUES (
         1,
		 @tt_name,
		 0,
	     @tt_muser,
         @tt_mdate,
         @tt_actual)
   END
END

/*      Return result and check         */
/*      existence of parent record      */
SELECT
   tt_code    = TaskType.tt_code,
   tt_name    = TaskType.tt_name
FROM  TaskType
WHERE TaskType.tt_mdate      = @tt_mdate
  AND TaskType.tt_muser      = @tt_muser
  AND TaskType.tt_deleted    = 0

/*      Finish transaction              */
IF (@@rowcount > 0) COMMIT   TRANSACTION
ELSE                ROLLBACK TRANSACTION

