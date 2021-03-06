SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER OFF
GO

CREATE PROCEDURE [dbo].[ReadOneStatusType]
(@st_code    udd_code)

AS
   SELECT 
      st_code      = StatusType.st_code,
      st_name      = StatusType.st_name,
      st_actual    = StatusType.st_actual,
      st_mdate     = StatusType.st_mdate,
      st_muser     = Options.opt_name_first,
      st_read_only = ISNULL(TrustedUser.tru_user - USER_ID(), 1)
   FROM  StatusType,
         TrustedUser,
         Options
   WHERE   StatusType.st_code      = @st_code
       AND StatusType.st_deleted    = 0
       AND Options.opt_user         = StatusType.st_muser
       AND TrustedUser.tru_owner    =* Options.opt_user
       AND TrustedUser.tru_user     = USER_ID()