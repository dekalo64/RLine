USE [VL]
GO
/****** Объект:  StoredProcedure [dbo].[DeletePriorityType]    Дата сценария: 04/26/2013 11:44:34 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

--
-- Definition for stored procedure DeletePriorityType : 
--

--
-- Definition for stored procedure DeletePriorityType : 
--

CREATE PROCEDURE [dbo].[DeletePriorityType]
(@pt_code      udd_code)

AS

UPDATE PriorityType
SET   PriorityType.pt_deleted = 1,
      PriorityType.pt_mdate   = GETDATE()
FROM  TrustedUser
WHERE PriorityType.pt_code         = @pt_code
  AND TrustedUser.tru_owner = PriorityType.pt_muser
  AND TrustedUser.tru_user  = USER_ID()
/*      Return result           */
SELECT deleted = @@rowcount
