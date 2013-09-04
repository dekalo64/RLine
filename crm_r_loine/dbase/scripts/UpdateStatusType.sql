USE [VL]
GO
/****** Объект:  StoredProcedure [dbo].[UpdateStatusType]    Дата сценария: 04/26/2013 11:29:42 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

--
-- Definition for stored procedure UpdateStatusType : 
--

--
-- Definition for stored procedure UpdateStatusType : 
--

CREATE PROCEDURE [dbo].[UpdateStatusType]
(@st_code              udd_code,
 @st_name              udd_name,
 @st_actual            udd_logical)

AS

BEGIN TRANSACTION

/*      Update data             */
UPDATE StatusType
SET    StatusType.st_name     = @st_name,
       StatusType.st_actual   = @st_actual,
       StatusType.st_mdate    = GETDATE()
WHERE  StatusType.st_code     = @st_code
  AND  StatusType.st_deleted  = 0

/*      Return result and check         */
/*      existence of parent record      */
SELECT
   st_code = StatusType.st_code
FROM  StatusType,
      TrustedUser
WHERE StatusType.st_code      = @st_code
  AND StatusType.st_deleted   = 0
  AND TrustedUser.tru_owner   = StatusType.st_muser
  AND TrustedUser.tru_user    = USER_ID()

/*      Finish transaction              */
IF (@@rowcount > 0) COMMIT   TRANSACTION
ELSE                ROLLBACK TRANSACTION
