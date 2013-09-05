USE [VL]
GO
/****** Объект:  StoredProcedure [dbo].[UpdateTaskType]    Дата сценария: 04/25/2013 23:04:19 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

--
-- Definition for stored procedure UpdateTaskType : 
--

--
-- Definition for stored procedure UpdateTaskType : 
--

CREATE PROCEDURE [dbo].[UpdateTaskType]
(@tt_code              udd_code,
 @tt_name              udd_name,
 @tt_actual            udd_logical)

AS

BEGIN TRANSACTION

/*      Update data             */
UPDATE TaskType
SET    TaskType.tt_name     = @tt_name,
       TaskType.tt_actual   = @tt_actual,
       TaskType.tt_mdate    = GETDATE()
WHERE  TaskType.tt_code     = @tt_code
  AND  TaskType.tt_deleted  = 0

/*      Return result and check         */
/*      existence of parent record      */
SELECT
   tt_code = TaskType.tt_code
FROM  TaskType,
      TrustedUser
WHERE TaskType.tt_code      = @tt_code
  AND TaskType.tt_deleted   = 0
  AND TrustedUser.tru_owner   = TaskType.tt_muser
  AND TrustedUser.tru_user    = USER_ID()

/*      Finish transaction              */
IF (@@rowcount > 0) COMMIT   TRANSACTION
ELSE                ROLLBACK TRANSACTION
