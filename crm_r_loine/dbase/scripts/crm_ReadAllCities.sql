SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER OFF
GO

CREATE PROCEDURE [dbo].crm_ReadAllCities
(@cit_actual    udd_type,
 @cit_country   udd_code)

AS
BEGIN

DECLARE 
    @cit_country_name udd_name

SET @cit_country_name =(SELECT cty_name_first FROM Country C 
    					WHERE C.cty_code = @cit_country)

   SELECT
      cit_code         = City.cit_code,
      cit_country_name = @cit_country_name,
      cit_name_first   = City.cit_name_first,
      cit_name_second  = City.cit_name_second, 
      cit_phone_code   = City.cit_phone_code,
      cit_actual       = City.cit_actual
   FROM  
      City  
   WHERE City.cit_country = @cit_country
     AND City.cit_actual >= @cit_actual
     AND City.cit_deleted = 0
   ORDER BY
      City.cit_name_first
END
