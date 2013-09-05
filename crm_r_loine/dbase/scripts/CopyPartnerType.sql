USE [VL]
GO
/****** Объект:  StoredProcedure [dbo].[CopyPartnerType]    Дата сценария: 04/26/2013 09:55:18 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

--
-- Definition for stored procedure CopyPartnerType : 
--

--
-- Definition for stored procedure CopyPartnerType : 
--

CREATE PROCEDURE [dbo].[CopyPartnerType]
(@part_code udd_code)

AS

DECLARE @part_name     udd_name, 
        @part_actual   udd_logical

/*      Read city data          */
SELECT
   @part_name    = PartnerType.part_name + ' (K)',
   @part_actual  = PartnerType.part_actual
FROM  PartnerType
WHERE PartnerType.part_code    = @part_code
  AND PartnerType.part_deleted = 0

IF (@@rowcount > 0) BEGIN
   EXECUTE dbo.InsertPartnerType  @part_name,
                                  @part_actual
END
ELSE BEGIN
   SELECT
      part_code    = PartnerType.part_code,
      part_name    = PartnerType.part_name
   FROM  PartnerType
   WHERE 1 = 2
END
