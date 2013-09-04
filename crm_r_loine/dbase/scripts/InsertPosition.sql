USE [VL]
GO
/****** Объект:  StoredProcedure [dbo].[InsertPosition]    Дата сценария: 04/25/2013 10:47:31 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

--
-- Definition for stored procedure InsertPosition : 
--

--
-- Definition for stored procedure InsertPosition : 
--

CREATE PROCEDURE [dbo].[InsertPosition]
(@pos_name           udd_name,
 @pos_actual         udd_logical)

AS

/*      Set stamp variables     */
DECLARE @pos_mdate      udd_date,
        @pos_muser      udd_user

SELECT
   @pos_muser = USER_ID(),
   @pos_mdate = GETDATE()

BEGIN TRANSACTION

/*      Recall deleted record   */
UPDATE Positions
SET Positions.pos_name    = @pos_name,
	Positions.pos_muser   = @pos_muser,
	Positions.pos_mdate   = @pos_mdate,
	Positions.pos_deleted = 0,
    Positions.pos_actual  = @pos_actual
WHERE Positions.pos_code = (SELECT MIN(Positions.pos_code)
                       FROM  Positions
                       WHERE Positions.pos_deleted = 1)

/*      Insert new record       */
IF (@@rowcount < 1)
BEGIN
   INSERT INTO Positions
      SELECT
         pos_code        = Positions.pos_code + 1,
         pos_name        = @pos_name,
		 pos_muser       = @pos_muser,
         pos_mdate       = @pos_mdate,
         pos_deleted     = 0,
         pos_actual      = @pos_actual   
      FROM  Positions
      WHERE Positions.pos_code = (SELECT MAX(Positions.pos_code)
                             FROM Positions)

/*      Insert first record     */
   IF (@@rowcount < 1)
   BEGIN
      INSERT INTO Positions (
		 pos_code,
		 pos_name,
	     pos_muser,
         pos_mdate,
         pos_deleted,
         pos_actual)
      VALUES (
         1,
		 @pos_name,
	     @pos_muser,
         @pos_mdate,
         0,
         @pos_actual)
   END
END

/*      Return result and check         */
/*      existence of parent record      */
SELECT
   pos_code    = Positions.pos_code,
   pos_name    = Positions.pos_name
FROM  Positions
WHERE Positions.pos_mdate      = @pos_mdate
  AND Positions.pos_muser      = @pos_muser
  AND Positions.pos_deleted    = 0

/*      Finish transaction              */
IF (@@rowcount > 0) COMMIT   TRANSACTION
ELSE                ROLLBACK TRANSACTION

