SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER OFF
GO

--
-- Definition for stored procedure FindPosition : 
--

CREATE PROCEDURE [dbo].[FindPosition]
(@pos_actual    udd_type,
 @pos_name      udd_tinyname)

AS

SELECT
   pos_code   = Positions.pos_code,
   pos_name   = Positions.pos_name,
   pos_actual = Positions.pos_actual
FROM  Positions
WHERE Positions.pos_name       =  @pos_name
  AND Positions.pos_actual     >= @pos_actual
  AND Positions.pos_deleted    = 0
  AND (PATINDEX(@pos_name, Positions.pos_name)  != 0)