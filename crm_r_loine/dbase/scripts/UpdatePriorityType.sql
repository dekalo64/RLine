SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER OFF
GO


--
-- Definition for stored procedure UpdatePriorityType : 
--

--
-- Definition for stored procedure UpdatePriorityType : 
--

CREATE PROCEDURE [dbo].[UpdatePriorityType]
(@pt_code              udd_code,
 @pt_name              udd_name,
 @pt_actual            udd_logical,
 @pt_level             udd_code)

AS

BEGIN TRANSACTION

/*      Update data             */
UPDATE PriorityType
SET    PriorityType.pt_name     = @pt_name,
       PriorityType.pt_actual   = @pt_actual,
       PriorityType.pt_level    = @pt_level,
       PriorityType.pt_mdate    = GETDATE()
WHERE  PriorityType.pt_code     = @pt_code
  AND  PriorityType.pt_deleted  = 0

/*      Return result and check         */
/*      existence of parent record      */
SELECT
   pt_code = PriorityType.pt_code
FROM  PriorityType,
      TrustedUser
WHERE PriorityType.pt_code      = @pt_code
  AND PriorityType.pt_deleted   = 0
  AND TrustedUser.tru_owner   = PriorityType.pt_muser
  AND TrustedUser.tru_user    = USER_ID()

/*      Finish transaction              */
IF (@@rowcount > 0) COMMIT   TRANSACTION
ELSE                ROLLBACK TRANSACTION
