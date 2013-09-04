SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER OFF
GO

CREATE PROCEDURE [dbo].[ReadAllCountry]
(@cty_actual    udd_type,
 @cty_skip      udd_code)

AS

SELECT
   cty_code        = Country.cty_code,
   cty_parent      = Country.cty_parent,
   cty_name_first  = Country.cty_name_first,
   cty_name_second = Country.cty_name_second,
   cty_phone_code  = Country.cty_phone_code,
   cty_actual      = Country.cty_actual
FROM  Country
WHERE Country.cty_deleted = 0
  AND Country.cty_code   != Country.cty_parent
  AND Country.cty_code   != @cty_skip
  AND Country.cty_actual >= @cty_actual
ORDER BY
   Country.cty_name_first
