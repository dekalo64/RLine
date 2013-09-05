USE [VL]
GO
/****** Объект:  StoredProcedure [dbo].[InsertPartnerType]    Дата сценария: 04/26/2013 09:50:21 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

--
-- Definition for stored procedure InsertPartnerType : 
--

--
-- Definition for stored procedure InsertPartnerType : 
--

CREATE PROCEDURE [dbo].[InsertPartnerType]
(@part_name           udd_name,
 @part_actual         udd_logical)

AS

/*      Set stamp variables     */
DECLARE @part_mdate      udd_date,
        @part_muser      udd_user

SELECT
   @part_muser = USER_ID(),
   @part_mdate = GETDATE()

BEGIN TRANSACTION

/*      Recall deleted record   */
UPDATE PartnerType
SET PartnerType.part_name     = @part_name,
	PartnerType.part_muser    = @part_muser,
	PartnerType.part_mdate    = @part_mdate,
	PartnerType.part_deleted  = 0,
    PartnerType.part_actual   = @part_actual
WHERE PartnerType.part_code = (SELECT MIN(PartnerType.part_code)
                       FROM  PartnerType
                       WHERE PartnerType.part_deleted = 1)

/*      Insert new record       */
IF (@@rowcount < 1)
BEGIN
   INSERT INTO PartnerType
      SELECT
         part_code        = PartnerType.part_code + 1,
         part_name        = @part_name,
		 part_deleted     = 0,
		 part_muser       = @part_muser,
         part_mdate       = @part_mdate,
         part_actual      = @part_actual   
      FROM  PartnerType
      WHERE PartnerType.part_code = (SELECT MAX(PartnerType.part_code)
                             FROM PartnerType)

/*      Insert first record     */
   IF (@@rowcount < 1)
   BEGIN
      INSERT INTO PartnerType (
		 part_code,
		 part_name,
		 part_deleted,
	     part_muser,
         part_mdate,
         part_actual)
      VALUES (
         1,
		 @part_name,
		 0,
	     @part_muser,
         @part_mdate,
         @part_actual)
   END
END

/*      Return result and check         */
/*      existence of parent record      */
SELECT
   part_code    = PartnerType.part_code,
   part_name    = PartnerType.part_name
FROM  PartnerType
WHERE PartnerType.part_mdate      = @part_mdate
  AND PartnerType.part_muser      = @part_muser
  AND PartnerType.part_deleted    = 0

/*      Finish transaction              */
IF (@@rowcount > 0) COMMIT   TRANSACTION
ELSE                ROLLBACK TRANSACTION

