SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER OFF
GO

CREATE PROCEDURE [dbo].[ReadOnePosition]
(@pos_code    udd_code)

AS
   SELECT 
      pos_code      = Positions.pos_code,
      pos_name      = Positions.pos_name,
      pos_actual    = Positions.pos_actual,
      pos_mdate     = Positions.pos_mdate,
      pos_muser     = Options.opt_name_first,
      pos_read_only = ISNULL(TrustedUser.tru_user - USER_ID(), 1)
   FROM  Positions,
         TrustedUser,
         Options
   WHERE   Positions.pos_code      = @pos_code
       AND Positions.pos_deleted    = 0
       AND Options.opt_user         = Positions.pos_muser
       AND TrustedUser.tru_owner    =* Options.opt_user
       AND TrustedUser.tru_user     = USER_ID()