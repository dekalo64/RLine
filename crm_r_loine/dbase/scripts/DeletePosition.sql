USE [VL]
GO
/****** Объект:  StoredProcedure [dbo].[DeletePosition]    Дата сценария: 04/25/2013 14:08:14 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

--
-- Definition for stored procedure DeletePosition : 
--

--
-- Definition for stored procedure DeletePosition : 
--

CREATE PROCEDURE [dbo].[DeletePosition]
(@pos_code      udd_code)

AS

UPDATE Positions
SET   Positions.pos_deleted = 1,
      Positions.pos_mdate   = GETDATE()
FROM  TrustedUser
WHERE Positions.pos_code         = @pos_code
  AND TrustedUser.tru_owner = Positions.pos_muser
  AND TrustedUser.tru_user  = USER_ID()
/*      Return result           */
SELECT deleted = @@rowcount
