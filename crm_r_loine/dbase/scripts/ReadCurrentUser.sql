SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER OFF
GO

CREATE PROCEDURE [dbo].[ReadCurrentUser]
(@user_id udd_code)
AS
IF (@user_id > 0)
SELECT Options.opt_name_first
           FROM   Options
           WHERE  Options.opt_user = @user_id
ORDER BY
   Options.opt_name_first
ELSE
SELECT Options.opt_name_first
           FROM   Options
           WHERE  Options.opt_user = USER_ID()
ORDER BY
   Options.opt_name_first

  
