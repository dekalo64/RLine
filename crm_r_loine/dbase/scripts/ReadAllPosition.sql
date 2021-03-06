SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER OFF
GO

--
-- Definition for stored procedure ReadAllPosition : 
--

--
-- Definition for stored procedure ReadAllPosition : 
--

CREATE PROCEDURE [dbo].[ReadAllPosition]
(@pos_actual    udd_type)

AS
   SELECT
      pos_code    = Positions.pos_code,
      pos_name    = Positions.pos_name,
      pos_actual  = Positions.pos_actual
   FROM  Positions
   WHERE Positions.pos_actual >= @pos_actual
     AND Positions.pos_deleted = 0
   ORDER BY
      Positions.pos_name
