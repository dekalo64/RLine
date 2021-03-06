SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER OFF
GO


--
-- Definition for stored procedure ReadAllPartnerType : 
--

--
-- Definition for stored procedure ReadAllPartnerType : 
--

CREATE PROCEDURE [dbo].[ReadAllPartnerType]
(@part_actual    udd_type)

AS
   SELECT 
      part_code    = PartnerType.part_code,
      part_name    = PartnerType.part_name,
      part_actual  = PartnerType.part_actual
   FROM  PartnerType
   WHERE PartnerType.part_actual >= @part_actual
     AND PartnerType.part_deleted = 0
   ORDER BY
      PartnerType.part_name
