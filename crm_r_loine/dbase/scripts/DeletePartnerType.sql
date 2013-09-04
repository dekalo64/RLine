USE [VL]
GO
/****** Объект:  StoredProcedure [dbo].[DeletePartnerType]    Дата сценария: 04/26/2013 10:08:40 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

--
-- Definition for stored procedure DeletePartnerType : 
--

--
-- Definition for stored procedure DeletePartnerType : 
--

CREATE PROCEDURE [dbo].[DeletePartnerType]
(@part_code      udd_code)

AS

UPDATE PartnerType
SET   PartnerType.part_deleted = 1,
      PartnerType.part_mdate   = GETDATE()
FROM  TrustedUser
WHERE PartnerType.part_code         = @part_code
  AND TrustedUser.tru_owner = PartnerType.part_muser
  AND TrustedUser.tru_user  = USER_ID()
/*      Return result           */
SELECT deleted = @@rowcount
