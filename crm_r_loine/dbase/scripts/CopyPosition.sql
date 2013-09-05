USE [VL]
GO
/****** Объект:  StoredProcedure [dbo].[CopyPosition]    Дата сценария: 04/25/2013 13:47:32 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

--
-- Definition for stored procedure CopyPosition : 
--

--
-- Definition for stored procedure CopyPosition : 
--

CREATE PROCEDURE [dbo].[CopyPosition]
(@pos_code udd_code)

AS

DECLARE @pos_name     udd_name, 
        @pos_actual   udd_logical

/*      Read city data          */
SELECT
   @pos_name    = Positions.pos_name + ' (K)',
   @pos_actual  = Positions.pos_actual
FROM  Positions
WHERE Positions.pos_code    = @pos_code
  AND Positions.pos_deleted = 0

IF (@@rowcount > 0) BEGIN
   EXECUTE dbo.InsertPosition  @pos_name,
                               @pos_actual
END
ELSE BEGIN
   SELECT
      pos_code    = Positions.pos_code,
      pos_name    = Positions.pos_name
   FROM  Positions
   WHERE 1 = 2
END
