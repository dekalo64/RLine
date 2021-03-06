SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER OFF
GO

CREATE PROCEDURE [dbo].[FindPartnerType]
(@part_actual    udd_type,
 @part_name      udd_tinyname)

AS

SELECT
   part_code   = PartnerType.part_code,
   part_name   = PartnerType.part_name,
   part_actual = PartnerType.part_actual
FROM  PartnerType
WHERE PartnerType.part_name       =  @part_name
  AND PartnerType.part_actual     >= @part_actual
  AND PartnerType.part_deleted    = 0
  AND (PATINDEX(@part_name, PartnerType.part_name)  != 0)
