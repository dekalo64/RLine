USE [VL]
GO
/****** Объект:  StoredProcedure [dbo].[UpdatePartnerType]    Дата сценария: 04/26/2013 10:17:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

--
-- Definition for stored procedure UpdatePartnerType : 
--

--
-- Definition for stored procedure UpdatePartnerType : 
--

CREATE PROCEDURE [dbo].[UpdatePartnerType]
(@part_code              udd_code,
 @part_name              udd_name,
 @part_actual            udd_logical)

AS

BEGIN TRANSACTION

/*      Update data             */
UPDATE PartnerType
SET    PartnerType.part_name     = @part_name,
       PartnerType.part_actual   = @part_actual,
       PartnerType.part_mdate    = GETDATE()
WHERE  PartnerType.part_code     = @part_code
  AND  PartnerType.part_deleted  = 0

/*      Return result and check         */
/*      existence of parent record      */
SELECT
   part_code = PartnerType.part_code
FROM  PartnerType,
      TrustedUser
WHERE PartnerType.part_code      = @part_code
  AND PartnerType.part_deleted   = 0
  AND TrustedUser.tru_owner   = PartnerType.part_muser
  AND TrustedUser.tru_user    = USER_ID()

/*      Finish transaction              */
IF (@@rowcount > 0) COMMIT   TRANSACTION
ELSE                ROLLBACK TRANSACTION
