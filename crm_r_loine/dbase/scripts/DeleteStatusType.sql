USE [VL]
GO
/****** Объект:  StoredProcedure [dbo].[DeleteStatusType]    Дата сценария: 04/26/2013 11:20:11 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

--
-- Definition for stored procedure DeleteStatusType : 
--

--
-- Definition for stored procedure DeleteStatusType : 
--

CREATE PROCEDURE [dbo].[DeleteStatusType]
(@st_code      udd_code)

AS

UPDATE StatusType
SET   StatusType.st_deleted = 1,
      StatusType.st_mdate   = GETDATE()
FROM  TrustedUser
WHERE StatusType.st_code         = @st_code
  AND TrustedUser.tru_owner = StatusType.st_muser
  AND TrustedUser.tru_user  = USER_ID()
/*      Return result           */
SELECT deleted = @@rowcount
