USE [VL]
GO
/****** Объект:  StoredProcedure [dbo].[UpdatePosition]    Дата сценария: 04/25/2013 15:05:57 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

--
-- Definition for stored procedure UpdatePosition : 
--

--
-- Definition for stored procedure UpdatePosition : 
--

CREATE PROCEDURE [dbo].[UpdatePosition]
(@pos_code              udd_code,
 @pos_name              udd_name,
 @pos_actual            udd_logical)

AS

BEGIN TRANSACTION

/*      Update data             */
UPDATE Positions
SET    Positions.pos_name     = @pos_name,
       Positions.pos_actual   = @pos_actual,
       Positions.pos_mdate    = GETDATE()
WHERE  Positions.pos_code     = @pos_code
  AND  Positions.pos_deleted  = 0

/*      Return result and check         */
/*      existence of parent record      */
SELECT
   pos_code = Positions.pos_code
FROM  Positions,
      TrustedUser
WHERE Positions.pos_code      = @pos_code
  AND Positions.pos_deleted   = 0
  AND TrustedUser.tru_owner   = Positions.pos_muser
  AND TrustedUser.tru_user    = USER_ID()

/*      Finish transaction              */
IF (@@rowcount > 0) COMMIT   TRANSACTION
ELSE                ROLLBACK TRANSACTION
