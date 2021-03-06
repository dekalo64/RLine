SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER OFF
GO

--
-- Definition for stored procedure crm_FindCities : 
--

--
-- Definition for stored procedure crm_FindCities : 
--

CREATE PROCEDURE [dbo].crm_FindCities
(@cit_actual    udd_type,
 @cit_name      udd_tinyname)

AS

SELECT
      cit_code         = City.cit_code,
      cit_country_name = Country.cty_name_first,
      cit_name_first   = City.cit_name_first,
      cit_name_second  = City.cit_name_second, 
      cit_phone_code   = City.cit_phone_code,
      cit_actual       = City.cit_actual
FROM  City,
      Country
WHERE City.cit_actual    >= @cit_actual
  AND City.cit_deleted    = 0
  AND Country.cty_code    = City.cit_country
  AND Country.cty_actual >= @cit_actual
  AND Country.cty_deleted = 0
  AND (PATINDEX(@cit_name, City.cit_name_first)  != 0
   OR  PATINDEX(@cit_name, City.cit_name_second) != 0)
ORDER BY
   City.cit_country,
   City.cit_name_first
